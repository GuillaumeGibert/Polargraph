function [x, y] = imageRefFrame2WorldRefFrame(i, j, theta, tx, ty)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function [x, y] = imageRefFrame2WorldRefFrame(i, j, theta, tx, ty)
% Task: computes the 2D coordinates of a point in the world reference frame given its position in the image frame
%
% Inputs:
%	- i: x-axis value of the target point in the image reference frame
%	- j: y-axis value of the target point in the image reference frame
%	- theta: rotation angle between the 2 reference frames
%	- tx: translation along x-axis between the 2 reference frames
%	- ty: translation along y-axis between the 2 reference frames
%
% Outputs: 
%	-x: x-coordinate the target point in the world reference frame
%	-y: y-coordinate the target point in the world reference frame
%
%
% author: Guillaume Gibert, guillaume.gibert@ecam.fr
% date: 04/12/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% converts theta to rad
theta_rad = theta / 180.0 * pi;

% creates the transformation matrix
transformationMatrix = [	cos(theta_rad) 	-sin(theta_rad) 	tx;
					sin(theta_rad)	cos(theta_rad)	ty;
					0			0			1];
					
					
% performs the transformation
pointImageSpace = [i; j; 1];

pointWorldSpace = transformationMatrix * pointImageSpace;

x = pointWorldSpace(1);
y = pointWorldSpace(2);