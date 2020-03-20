function out = runCSSR(alphabet, data, maxlength, ofilename, siglevel, test_mode)
%__________________________________________________________________________
% INPUTS
%   - alphabet, is the char array which contains all symbols used in the input data stream
%   - data, input data time series. It could contain more than one line.
%   - maxlength,  is the maximum history length considered in causal states. 
%   - siglevel, is the significance level.
%   - test_mode, significance level, either 'ks' or 'chi'
%   - ofilename, the name of output files.
%
% OUTPUTS
%   - out.Cmu, the statistical complexity. 
%   - out.state_prob, the stationary probability distribution of each causal states. 
%   - out.state_series, the series of causal states in the data. 
%
%--------------------------------------------------------------------------
% This function was written by Haohua Li @tlab on 19/12/2019. 
% tLab         <https://sites.google.com/monash.edu/tlab/home>
% 
% For related documentations, please visit
% <https://github.com/randoruf/CSSR-Matlab>
%--------------------------------------------------------------------------
% This program is free software: you can redistribute it and/or modify it under
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
%__________________________________________________________________________

% set default arguments
% (alphabet, data, maxlength, ofilename, siglevel, test_mode)
if nargin == 3
    ofilename = 'cssr';
    siglevel = 0.001;
    test_mode = 'ks'; 
elseif nargin == 4
    siglevel = 0.001;
    test_mode = 'ks'; 
elseif nargin == 5
    test_mode = 'ks'; 
end

    
% determine whether to read text file
if iscell(data) || ischar(data)
    % if pass a file name
    if ischar(data)
       i = 1; 
       fid = fopen(data, 'r');
       data = {}; 
       coder.varsize('data');
       while ~feof(fid)
           tline = fgetl(fid);
           data{i, 1} = tline;
           i = i + 1;
       end 
       fclose(fid); 
    end
else
    error('Input data must be a cell array or string.')
end

% execute CSSR
try
  % statistical complexity and state probabilities
  [out.Cmu, out.state_prob] = CSSR(ofilename, alphabet, data, maxlength, siglevel, test_mode); 
  % read the output state series file
  fid  = fopen([ofilename '_state_series.txt'], 'r');
  out.state_series = cell(size(data));
  expression = '(\d+(\.\d+)?)'; % filter intgers or floats
  i = 1; 
  while ~feof(fid)
    tline = fgetl(fid);
    [match, ~] = regexp(tline, expression, 'match', 'split'); % regular expression to filter integers.
    nan_states = repmat({'?'}, 1, length(data{i,1}) - size(match,2));
    out.state_series{i,1} = [nan_states, match{:}];  
    i = i + 1;
  end 
  fclose(fid); 
  % generate the fancy epsilon machine
  visualise_epsilon_machine([ofilename '_inf.dot'])
catch ME
    switch ME.identifier
        case 'MATLAB:UndefinedFunction'
            error('CSSR is undefined. Please run "makeCSSR.m" to compile first and then run again.');
        otherwise
            rethrow(ME)
    end
end


end