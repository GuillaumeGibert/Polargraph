function [motorIncrementA, motorIncrementB] = chainLengthDelta2MotorIncrement(ACdelta, BCdelta)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function [motorIncrementA, motorIncrementB] = chainLengthDelta2MotorIncrement(ACdelta, BCdelta)
% Task: estimate the motor increment given a delta in the chain lengths BC and AC
%
% Inputs:
%	- ACdelta: delta change in the chain AC length
%	- BCdelta: delta change in the chain BC length
%
% Outputs: 
%	-motorIncrementA: increment of motor A
%	-motorIncrementB: increment of motor B
%
%
% author: Guillaume Gibert, guillaume.gibert@ecam.fr
% date: 01/12/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% load config information
config

% estimate the angle to performed by motor A and the corresponding motor increment
angleMotorA = ACdelta / gearARadius;
motorIncrementA = angleMotorA / motorAIncrementCalibrationFactor;

% estimate the angle to performed by motor B and the corresponding motor increment
angleMotorB = BCdelta / gearBRadius;
motorIncrementB = angleMotorB / motorAIncrementCalibrationFactor;
