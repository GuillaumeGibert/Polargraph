function kinematicSimulation(simCase)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function kinematicSimulation(simCase)
% Task: perform kinematic simulation of the 2D plotter
%
% Input:
%	- simCase: the simulation case to launch
%			*0->the plotter goes to the center of the board
%			*1->the plotter moves along an horizontal line
%			*2->the plotter moves along a circle centerd at the center of the board
%
% Output: None
%
%
% author: Guillaume Gibert, guillaume.gibert@ecam.fr
% date: 01/12/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% close all
close all

% load the config information
config

% create a figure
figure;

% plot C at the center of the board
if (simCase == 0)
	fprintf('C will move to the middle of the board\n')
	
	%estimate the length of AC and BC for the starting point
	[AC, BC] = targetCoord2ChainLength(boardSize(1)/2, boardSize(2)/2, Apos(1), Bpos(1), Apos(2), Bpos(2));
	ACp = AC;
	BCp = BC;
	
	Cpos = [boardSize(1)/2 boardSize(2)/2];
	moveC(Cpos(1), Cpos(2), ACp, BCp);
end

% move C along a horizontal line
if (simCase == 1)
	fprintf('C will move along a horizontal line at the bottom of the board\n')
	
	% store motor increment history and AC/BC length history
	motorIncrementAHistory = [];
	motorIncrementBHistory = [];
	ACLengthHistory = [];
	BCLengthHistory = [];
	
	%estimate the length of AC and BC for the starting point
	[AC, BC] = targetCoord2ChainLength(Apos(1), 10, Apos(1), Bpos(1), Apos(2), Bpos(2));
	ACp = AC;
	BCp = BC;
	
	%loop over all the values
	for l_x=Apos(1):Bpos(1)
		Cpos = [l_x 10];
		[AC, BC,  motorIncrementA, motorIncrementB] = moveC(Cpos(1), Cpos(2), ACp, BCp);
		ACp = AC;
		BCp = BC;
		motorIncrementAHistory = [motorIncrementAHistory motorIncrementA];
		motorIncrementBHistory = [motorIncrementBHistory motorIncrementB];
		ACLengthHistory = [ACLengthHistory AC];
		BCLengthHistory = [BCLengthHistory BC];
		pause(0.1)
	end
	
	figure;
	title('Variation of motor increment while following a horizontal line');
	subplot(2,1,1)
		plot(motorIncrementAHistory);
		xlabel('frame');
		ylabel('motor A increment (a.u.)');
		xlim([1 length(motorIncrementAHistory)])
	subplot(2,1,2)
		plot(motorIncrementBHistory);
		xlabel('frame');
		ylabel('motor B increment (a.u.)');
		xlim([1 length(motorIncrementBHistory)])
		
	figure;
	title('Variation of AC & BC lengths while following a horizontal line');
	subplot(2,1,1)
		plot(ACLengthHistory);
		xlabel('frame');
		ylabel('AC length (cm)');
		xlim([1 length(ACLengthHistory)])
	subplot(2,1,2)
		plot(BCLengthHistory);
		xlabel('frame');
		ylabel('BC length (cm)');
		xlim([1 length(BCLengthHistory)])
	
end

% move C along a circle
if (simCase == 2)
	fprintf('C will move along a circle centered at the center of the board\n')
	
	% store motor increment history and AC/BC length history
	motorIncrementAHistory = [];
	motorIncrementBHistory = [];
	ACLengthHistory = [];
	BCLengthHistory = [];
	
	%estimate the length of AC and BC for the starting point
	[AC, BC] = targetCoord2ChainLength((boardSize(1)/2 + circleRadius*cos(0.0)), (boardSize(2)/2 + circleRadius*sin(0.0)), Apos(1), Bpos(1), Apos(2), Bpos(2));
	ACp = AC;
	BCp = BC;
	
	%loop over all the values
	for l_ang=0:360
		Cpos = [(boardSize(1)/2 + circleRadius*cos(l_ang/180.0*pi)) (boardSize(2)/2 + circleRadius*sin(l_ang/180.0*pi))];
		[AC, BC,  motorIncrementA, motorIncrementB] = moveC(Cpos(1), Cpos(2), ACp, BCp);
		ACp = AC;
		BCp = BC;
		motorIncrementAHistory = [motorIncrementAHistory motorIncrementA];
		motorIncrementBHistory = [motorIncrementBHistory motorIncrementB];
		ACLengthHistory = [ACLengthHistory AC];
		BCLengthHistory = [BCLengthHistory BC];
		pause(0.01)
	end
	
	figure;
	title('Variation of motor increment while following a horizontal line');
	subplot(2,1,1)
		plot(motorIncrementAHistory);
		xlabel('frame');
		ylabel('motor A increment (a.u.)');
		xlim([1 length(motorIncrementAHistory)])
	subplot(2,1,2)
		plot(motorIncrementBHistory);
		xlabel('frame');
		ylabel('motor B increment (a.u.)');
		xlim([1 length(motorIncrementBHistory)])
		
	figure;
	title('Variation of AC & BC lengths while following a horizontal line');
	subplot(2,1,1)
		plot(ACLengthHistory);
		xlabel('frame');
		ylabel('AC length (cm)');
		xlim([1 length(ACLengthHistory)])
	subplot(2,1,2)
		plot(BCLengthHistory);
		xlabel('frame');
		ylabel('BC length (cm)');
		xlim([1 length(BCLengthHistory)])
end