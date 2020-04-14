function [alphabet, outData] = diff_binarise(inData)
% ________________________________________________________________
% diff_binarise 
%   Generate a binary string according to the differences between 
%   every two points. 
% 
% i = 2, 3 ,..., length(inData) 
%             | 0 , if inData[i] - inData[i-1] <= 0
% outData[i]  |  
%             | 1 , otherwise
% 
%---INPUTS:
%   inData, a MxN matrix of a group of time series. Assume the each row is 
%           an independent time series. 
%---OUTPUTS:
%   alphabet, the set of all characters that appear in the symbolised
%             sequence.
%   outData, a MxN matrix of a group binary strings.  
% ----------------------------------------------------------------
% This function is written by Haohua Li, 2020. 
% Visit my github <https://github.com/randoruf>
% ----------------------------------------------------------------
% This function is free software: you can redistribute it and/or modify it under
% the terms of the GNU General Public License as published by the Free Software
% Foundation, either version 3 of the License, or (at your option) any later
% version.
%
% This program is distributed in the hope that it will be useful, but WITHOUT
% ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
% details.
%
% You should have received a copy of the GNU General Public License along with
% this program. If not, see <http://www.gnu.org/licenses/>.
% ________________________________________________________________


% shift the inData by 1 bit and then get the differences array. 
d = inData(:, 2:end) - inData(:, 1:end-1);  
% or other implementation as d = diff(inData, 1, 2); 
% generate the binary string 
outData = d > 0;
outData = char(outData + 65);

alphabet = 'AB'; 
end 