function [C,R] = incircle(x,y)
% incircle: Berechnet den Größten Kreis innerhalb einer vorgegeben konvexen
% Hülle.
%
% [C,R] = incircle(x,y)
% Input sind x und y Werte der konvexen Hülle
% berechnet eine 2-Dim konvexe Hülle und findet einen max.
% Radius, welcher komplett in der konvexen Hülle liegt.
%
% [C,R] = incircle(xy)
% Input sind xy Werte einer List der konvexen Hülle
% berechnet eine 2-Dim konvexe Hülle und findet einen max.
% Radius, welcher komplett in der konvexen Hülle liegt.
%
%
% Rückgabewert ein 2-Dim Vektor, definiert den Mittelpunkt
% m (m_x, m_y) des Kreises sowie der Radius r des innerhalb liegenden
% Kreises.
%


if (nargin == 2) && isvector(x) && isvector(y) && (numel(x) == numel(y))
  % fügt die Werte zu einem pärchen zusammen
  xy = [x(:),y(:)];  
  
  % berechnet die konvexe Hülle und gibt die einzelnen Indizes zurück
  ecken = convhulln(xy);
elseif (nargin == 1) && (size(x,2) == 2)
  % a single list of points as rows of x
  xy = x;
  ecken = convhulln(xy);
else
    disp('Input nicht zulaessig.');
    return;  
end
% Anzahl der Strecken
strecken_nr = size(ecken,1);

% Startpunkt jeder Strecke
%%disp('Startpunkte.');
A = xy(ecken(:,1),:);
% Endpunkt jeder Strecke
%%disp('Endpunkte.');
B = xy(ecken(:,2),:);

% Der Normalenvektor zu jeder Strecke
% Richtungsvektor der Strecke um +90 Grad cw drehen
N_p = (B - A) * [0 1; -1 0];

% Komponentenweise, berechne den Betrag jeder Zeile (2 = Zeilensumme)
Betrag = sqrt(sum(N_p.^2,2));
% Komponentenweise, berechen den normierten Normalenvektor
%%disp('Normierte Normalenvektor.');
N_p = N_p./[Betrag, Betrag];

% Ein Mittelpunkt innerhalb der konvexen Hülle (1 = Spaltensumme)
% M_0 Mittelwert über alle x und y
M_0 = mean(A,1);

% Prüfe ob die Normalenvektoren nach innen zeigen, wenn
% nein, dann drehe sie nach innen
% Zeilenweise M_0 - A
% Komponentenweise Multiplikation
index = sum(N_p.*bsxfun(@minus, M_0, A), 2) < 0;
N_p(index,:) = -N_p(index, :);

% Vorbereitungen sind abgeschlossen, jetzt muss das
% LP Problem gelöst werden. Also die Gleichung aufstellen
%
% Ausgangspunkt:
% N_p: normierte Normalenvektor aller Strecken, Matrix 1. Zeile x,y einer
% Strecke
% Mittelpunkt: Der zu berechnende Mittelpunkt (x,y)
% Startpunkt: Startpunkt aller Polygon, Matrix 1. Zeile (x,y)
% N_p * [ Mittelpunkt - Startpunkt ] >= r
% --> -N_p * [ Mittelpunkt - Startpunkt ] + r <= 0
% --> -N_p * Mittelpunkt + r <= -N_p * Startpunkt

% Rechte Seite der Ungleichung
% -N_pi * Startpunkt_i
b = -sum(N_p.*A, 2);

% Matrix A: -N_p * Mittelpunkt + r
A = [-N_p.*ones(strecken_nr, 2), ones(strecken_nr, 1)];


% Lineare Zielfunktion -R
% Die ersten beiden Variablen beinhalten den Mittelpunkt
% die 3. den Radius.
f = zeros(3, 1);
% Zielfunktion -r
f(3) = -1;

%%options = optimset('LargeScale','off','Simplex','on');
options = optimset('linprog');
options.Display = 'off';
[result,fval,exitflag, output] = linprog(f,A,b,[],[],[],[],[],options);
%%[result, fval, exitflag, output] = linprog(f, A, b);
% Ergebnis auswerten
C = result(1:2)';
R = result(3); 