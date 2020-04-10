%=========================================================================
% CSSR Toolbox Installation (Makefile)
%   CSSR source code    - http://bactra.org/CSSR/ 
%   TsuchiyaLab(tlab)   - https://sites.google.com/monash.edu/tlab/home 
% ________________________________________________________________________
% Compilations had been tested in various conditions. 
%   - gcc 7.4, Ubuntu Linux 
%   - CLang 11.0, macOS
%   - Microsoft Visual C++ , MSVC 19.23.28106.4, Windows 
% _________________________________________________________________________
% Copyright (C) 2020 Haohua Li
%
% This program is free software; you can redistribute it and/or
% modify it under the terms of the GNU General Public License
% as published by the Free Software Foundation; either version 2
% of the License, or (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program; if not, write to the Free Software
% Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
%=========================================================================

% change directory to the path of current script (namely make.m) 
cd(fileparts(which(mfilename)));
% CSSR home directory 
mfile_name       = mfilename('fullpath');
[cssr_path,~,~]  = fileparts(mfile_name);

% Create objects files (without linking) 
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','Hash.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','States.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','AllStates.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','ParseTree.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','G_Array.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','Hash2.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','Machine.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','TransTable.cpp')])
eval(['mex -c -outdir cpp ' fullfile(cssr_path, 'cpp','Test.cpp')])

% link object files to a single program : CSSR 
if ispc 
   eval('mex -output CSSR cpp/Main.cpp cpp/hash.obj cpp/states.obj cpp/allStates.obj cpp/parsetree.obj cpp/g_array.obj cpp/hash2.obj cpp/machine.obj cpp/transtable.obj cpp/test.obj') 
elseif isunix || ismac 
   eval('mex -output CSSR cpp/Main.cpp cpp/hash.o cpp/states.o cpp/allStates.o cpp/parsetree.o cpp/g_array.o cpp/hash2.o cpp/machine.o cpp/transtable.o cpp/test.o') 
else 
   error('Platform not supported. (Only for Liunx, macOS and Windows)')
end 

% inform the user that CSSR has been successfully generated. 
disp('CSSR has been successfully compiled.')

