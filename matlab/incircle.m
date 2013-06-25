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
  edges = convhulln(xy);
elseif (nargin == 1) && (size(x,2) == 2)
  % a single list of points as rows of x
  xy = x;
  edges = convhulln(xy);
else
    disp('Input nicht zulaessig.');
    return;  
end
% Anzahl der Strecken
ne = size(edges,1);

% Startpunkt jeder Strecke
A = xy(edges(:,1),:);
% Endpunkt jeder Strecke
B = xy(edges(:,2),:);

% Der Normalenvektor zu jeder Strecke
% Richtungsvektor der Strecke um 90 Grad cw drehen
N = (B - A)*[0 1;-1 0];

% Komponentenweise, berechne den Betrag jeder Zeile (2 = Zeilensumme)
L = sqrt(sum(N.^2,2));
% Komponentenweise, berechen den normierten Normalenvektor
N = N./[L,L];

% Ein Mittelpunkt innerhalb der konvexen Hülle (1 = Spaltensumme)
% M_0 Mittelwert über alle x und y
M_0 = mean(A,1);

% Prüfe ob die Normalenvektoren nach innen zeigen, wenn
% nein, dann drehe sie nach innen
% Zeilenweise M_0 - A
% Komponentenweise Multiplikation
k = sum(N.*bsxfun(@minus,M_0,A),2) < 0;
N(k,:) = -N(k,:);

% LP-Problem
% Matrix für lineare Nebenbedingungen.
% N = normierter Normalvektor der Koordinaten
% Nuller für den Radius
% -1 für die Slak-Variablen
Aeq = [N,zeros(ne,1),-eye(ne)];
% Vektor für lineare Nebenbedingungen.
% Zeilen in beq müssen gleich Zeilen in Aeq
% normierte Startpunktkoordinaten
beq = sum(N.*A,2);

% Vektor für die unteren Schranken
% untere Grenze für Mittelpunkt (m_x,m_y)
% Radius sowie die Slack-Variablen (>=0)
LB = [-inf;-inf;0;zeros(ne,1)];
% Vektor für die oberen Schranken
UB = [];

% Ungleichheiten durch die Slack-Variablen Beschränkung
% Kreismittelpuunkt (m_x, m_y)
% Radius
% -1 Sklack-Varibalen
A = [zeros(ne,2),ones(ne,1),-eye(ne)];
% Wegen Slack-Variablen --> LGS b=0
b = zeros(ne,1);

% Lineare Zielfunktion -R
% Die ersten beiden Variablen beinhalten den Mittelpunkt
% die 3. den Radius, Rest für Slack-Variablen
f = zeros(ne+3,1);
% Zielfunktion -r
f(3) = -1;

options = optimset('linprog');
options.Display = 'off';
[result,fval,exitflag] = linprog(f,A,b,Aeq,beq,LB,UB,[],options);

% Ergebnis auswerten
C = result(1:2)';
R = result(3); 