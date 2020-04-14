function [alphabet, outData] = symbolic_transfer_discretiser(inData, order, tau)
%________________________________________________________________________________
% symbolic_transfer_discretiser 
%   Generate a symbolised sequence using the symbolic transformation Ref[4].
%------------------------------------------------------------------------------
%---INPUTS:
%   inData, the time series.
%   order,  the order of the permutation patterns.
%   tau,    the time-delay for the embedding.
%
%---OUTPUTS:
%   alphabet, the set of all characters that appear in the symbolised
%             sequence.
%   outData,  the symbolised sequence. 
%------------------------------------------------------------------------------
% Modified from EN_PermEn.m in HCTSA library. 
% The source code https://github.com/benfulcher/hctsa/blob/master/Operations/EN_PermEn.m
% The author of HCTSA: Ben Fulcher (ben.d.fulcher@gmail.com). 
%-------------------------------------------------------------------------------
% REFERENCE
% [1] B. D. Fulcher and N. S. Jones, 'hctsa?: A Computational Framework for Automated Time-Series Phenotyping Using Massive Feature Extraction', Cell Systems, vol. 5, no. 5, pp. 527-531.e3, Nov. 2017, doi: 10.1016/j.cels.2017.10.001.
% [2] B. D. Fulcher, M. A. Little, and N. S. Jones, 'Highly comparative time-series analysis: the empirical structure of time series and their methods', J. R. Soc. Interface, vol. 10, no. 83, p. 20130048, Jun. 2013, doi: 10.1098/rsif.2013.0048.
% [3] N. Nicolaou and J. Georgiou, 'Detection of epileptic electroencephalogram based on Permutation Entropy and Support Vector Machines', Expert Systems with Applications, vol. 39, no. 1, pp. 202-209, Jan. 2012, doi: 10.1016/j.eswa.2011.07.008. 
% [4] J.-R. King et al., 'Information Sharing in the Brain Indexes Consciousness in Noncommunicative Patients', Current Biology, vol. 23, no. 19, pp. 1914-1919, Oct. 2013, doi: 10.1016/j.cub.2013.07.075.
%-------------------------------------------------------------------------------
% This function is written by Haohua Li, 2020. 
% Visit my github <https://github.com/randoruf>
%-------------------------------------------------------------------------------
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
%________________________________________________________________________________

% check inputs and set defaults
if nargin == 1
    order = 2; 
    tau   = 1;  % For EEG, tau = 1 is sufficient for most cases. see Ref[3]
else
    % check order and tau 
    if order - round(order) ~= 0 || order < 1
        error('Order of ordinal patterns must be a postive integer.')
    end 
    if order > 2
        error('Order of ordinal patterns must be less than or equal to 2(modify this limit in "symbolic_transfer_discretiser.m").')
    end 
    if tau - round(tau) ~= 0 || tau < 1 
        error('Time-delay must be a postive integer.')
    end 
end 

% check if input series is valid 
if size(inData, 1) ~= 1 
    error('Input data must be a 1-dimensional string!')
end
if size(inData, 2) < tau*order + 1 
    error('Input time series is too short (not enough samples). Try to decrease time-delay or use a longer time series.')
end 

% full alphabet (but use up to the 24th letter)
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
num_pattern_points  = order + 1;
num_patterns = factorial(num_pattern_points);
alphabet = alphabet(1:num_patterns);  % truncate the full alphabet 

% existing ordinal patterns  
permutation_list = perms(1:order+1); 
% permutation_list

% generate a symbolised sequence according to the definition from
% permutation entropy. See Ref[4]
c_pattern = zeros(1, num_pattern_points);   % pre-allocate memory for c_pattern 
outData   = blanks(length(inData)-tau*order-1); 
outData_i = 0;  

% start from the next pattern X(t0+1)
for i = (order*tau+1) : 1 : length(inData)   
    % rank by amplitude
    [~,I] = sort(inData(i-order*tau : tau : i));
    c_pattern(I) = 1:num_pattern_points; 
    % c_pattern
    
    % find the corresponded pattern 
    for k = 1 : num_patterns
        % matching patterns (Ben's implementation), see Ref[1,2]
        if all(permutation_list(k,:)-c_pattern == 0)  
            outData_i = outData_i + 1; 
            outData(outData_i) = alphabet(k);  
            break 
        end
    end
    
    % the end of iteratoon...
end 

end 