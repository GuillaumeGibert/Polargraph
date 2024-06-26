function  [AC, BC, motorIncrementA, motorIncrementB] = moveC(xC, yC, ACp, BCp)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function moveC(xC, yC)
% Task: move C to a given target position
%
% Inputs:
%	- x: x-axis value of the target point
%	- y: y-axis value of the target point 
%	- ACp:  length of the previous AC segment
%	- BCp:  length of the previous BC segment
%
% Outputs: 
%	- AC: length of the AC segment
%	- BC: length of the BC segment
%	- motorIncrementA: number of motor increment for motor A
%	- motorIncrementB: number of motor increment for motor B
%
%
% author: Guillaume Gibert, guillaume.gibert@ecam.fr
% date: 01/12/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% load config information
config

%plot points
	% A point
plot(Apos(1), Apos(2), 'ro'); hold on;
text(Apos(1), Apos(2) + 5, 'A');
	% B point
plot(Bpos(1), Bpos(2), 'ro');
text(Bpos(1), Bpos(2) + 5, 'B');
	% C point
plot(xC, yC, 'k*'); hold on;
text(xC, yC + 5, 'C');
	% board size
xlim([0 boardSize(1)])
ylim([0 boardSize(2)])
	% label & title
xlabel('board width (cm)');
ylabel('board height (cm)');
title('2D plotter simulator');

% estimate the length of the different segments
[AC, BC] = targetCoord2ChainLength(xC, yC, Apos(1), Bpos(1), Apos(2), Bpos(2));

% estimate the position of E
EA = EC - AC;
Epos = [Apos(1) Apos(2)-EA];

% estimate the position of F
FB = FC - BC;
Fpos = [Bpos(1) Bpos(2)-FB];

% estimate the number of motor increment to perform this movement
ACdelta = AC - ACp;
BCdelta = BC - BCp;
[motorIncrementA, motorIncrementB] = chainLengthDelta2MotorIncrement(ACdelta, BCdelta);

% plot the segment
	%BC
plot([Bpos(1) xC], [Bpos(2) yC], 'k');
	%AC
plot([Apos(1) xC], [Apos(2) yC], 'k');
	%EA
plot([Apos(1) Epos(1)], [Apos(2) Epos(2)], 'k');	
plot(Epos(1), Epos(2), 'go');
text(Epos(1) - 5, Epos(2), 'E');
	%FB
plot([Bpos(1) Fpos(1)], [Bpos(2) Fpos(2)], 'k');	
plot(Fpos(1), Fpos(2), 'go');
text(Fpos(1) + 5, Fpos(2), 'F');



% hold off to remove previous plot
hold off;
