function unitTests
%%%%%%%%%%%%%%%%%%%%
% function unitTests
% Task: Test all the functions created for the Workshop "Robotics & Automation"
%
% Inputs: None
%
% Output: None
%
%
% author: Guillaume Gibert, guillaume.gibert@ecam.fr
% date: 15/06/2021
%%%%%%%%%%%%%%%%%%%%

threshold = 1e-10;

%%%%%%%%%%%%%%%%%%%%
% test of imageRefFrame2WorldRefFrame function
disp('%%%%%%%%%%%%%%%%%')
disp('Testing imageRefFrame2WorldRefFrame')
nbTestPassed = 0;
nbTestTotal = 0;

testingMaterial = [
			0 0 -90 0 640 0 640;
			320 240 90 480 640 240 960
			];

for l_test=1:size(testingMaterial,1)
	i = testingMaterial(l_test,1); j = testingMaterial(l_test,2); theta = testingMaterial(l_test,3);
	tx = testingMaterial(l_test,4); ty = testingMaterial(l_test,5); 
	correctX = testingMaterial(l_test,6); correctY = testingMaterial(l_test,7);
	
	nbTestTotal += 1;
	
	[x, y] = imageRefFrame2WorldRefFrame(i, j, theta, tx, ty);
	if ((x-correctX).^2+(y-correctY).^2 < threshold)
		fprintf('Test #%d passed!\n', l_test)
		nbTestPassed += 1;
	else
		fprintf('Test #%d failed!\n', l_test)
	end
end
	
%%%%%%%%%%%%%%%%%%%%
% test of imageRefFrame2WorldRefFrame function
disp('%%%%%%%%%%%%%%%%%')
disp('Testing chainLengthDelta2MotorIncrement')

testingMaterial = [
			0 0 0 0 ;
			320 240 160 120 
			];

for l_test=1:size(testingMaterial,1)
	ACdelta = testingMaterial(l_test,1);
	BCdelta = testingMaterial(l_test,2);
	motorIncrementA = testingMaterial(l_test,3);
	motorIncrementB = testingMaterial(l_test,4);
	
	nbTestTotal += 1;
	
	[miA, miB] = chainLengthDelta2MotorIncrement(ACdelta, BCdelta);
	
	if ((miA-motorIncrementA).^2+(miB-motorIncrementB).^2 < threshold)
		fprintf('Test #%d passed!\n', l_test)
		nbTestPassed += 1;
	else
		fprintf('Test #%d failed!\n', l_test)
	end
end


% summary
disp('%%%%%%%%%%%%%%%%%')
disp('%SUMMARY%')
fprintf('# of tests passed: %d / %d\n', nbTestPassed, nbTestTotal)
disp('%%%%%%%%%%%%%%%%%')
