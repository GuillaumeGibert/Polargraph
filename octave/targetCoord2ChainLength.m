function [AC, BC] = targetCoord2ChainLength(x, y, xA, xB, yA, yB)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function [AC, BC] = targetCoord2ChainLength(x, y, xA, xB, yA, yB)
% Task: estimate the length of the chains given a target cartesian position
%
% Inputs:
%	- x: x-axis value of the target point
%	- y: y-axis value of the target point 
%	- xA: x-axis value of A point
%	- xB: x-axis value of B point 
%	- yA: y-axis value of A point
%	- yB: y-axis value of B point 
%
% Outputs: 
%	-AC: length of the AC segment
%	-BC: length of the BC segment
%
%
% author: Guillaume Gibert, guillaume.gibert@ecam.fr
% date: 01/12/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

AC = sqrt(x*x + y*y + yA*yA + xA*xA -2*xA*x -2*yA*y );

BC = sqrt(x*x + y*y + yB*yB + xB*xB -2*xB*x -2*yB*y );