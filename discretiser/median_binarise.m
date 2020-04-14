function [alphabet, outData] = median_binarise(inData)
% ________________________________________________________________
% median_binarise 
%    use median split to convert a time series into a binary string. 
%
% i = 1, 2 ,..., length(inData) 
%             | 0 , if inData[i] < median(inData)
% outData[i]  |  
%             | 1 , otherwise
% 
% ---INPUTS:
%   inData, a MxN matrix of a group of time series. Assume the each row is 
%           an independent time series. 
% ---INPUTS:
%   alphabet, the set of all characters that appear in the symbolised
%             sequence.
%   outData, a MxN matrix of a group binary strings.  
% ----------------------------------------------------------------
% This function is written by Haohua Li, 2020. 
% Visit my github <https://github.com/randoruf>
%-----------------------------------------------------------------
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

ref = median(inData);  
outData = inData > ref; 
outData = char(outData + 65); 
% outData

alphabet = 'AB'; 
end 