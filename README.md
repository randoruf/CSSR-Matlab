# CSSR-Matlab
[![View CSSR-Matlab on File Exchange](https://www.mathworks.com/matlabcentral/images/matlab-file-exchange.svg)](https://au.mathworks.com/matlabcentral/fileexchange/74604-cssr-matlab)

**Note that CSSR-Matlab is NOT an official distribution**. This program attempts to bind CSSR with Matlab such that CSSR can run within Matlab. 

Before using CSSR-Matlab, it is recommended to read documentations published in the CSSR official website http://www.bactra.org/CSSR. CSSR-Matlab is based on CSSR, the original algorithm CSSR is developed by [Cosma Shalizi](http://bactra.org/) and Kristina Klinkner. 

If you decide to use CSSR-Matlab, please appropriately cite the paper "Blind Construction of Optimal Nonlinear Recursive Predictors for Discrete Sequences",  [arXiv:cs/0406011 [cs.LG]](https://arxiv.org/abs/cs/0406011). 



## CSSR-Matlab Usage 

To run CSSR in Matlab, navigator to the directory of **CSSR-Matlab** and execute the following command

```matlab
out = runCSSR(alphabet, data, maxlength, ofilename, siglevel, test_mode)
```

* `alphabet` is the char array which contains all symbols used in the input data stream. 

  * ```matlab
    alphabet = 'ABC';
    alphabet = 'AB'; % or alphabet = '01';
    ```

* `data` is a collection of sequences of symbols (each sequence represents a distinct time series generated from the same stochastic process). If `data` has multiple rows, multiple-line mode will be turned on automatically. Note that data can be the file name of a text stream. 

  * ```matlab
    data = 'test1.txt'; % a text stream named 'test1.txt'
    data = {'01001110000111110011'}; % single-line 
    data = {'01111110111'; '01000010110'}; % multi-line and same length
    data = {'011110'; '000011111011010111011100110110'}; % multi-line but different length
    data = {'ABCBABABACCCABC'}; % use more than 2 characters (instead of binary representation)
    ```
    
  * Inside the text file `test1.txt`, the content looks like 

     * If `test1.txt` has single line 

       ```
       ABBBBBABABABCCCCABABABCCBBAAABABABCCCBABA
       ```

     * If `test1.txt` has multiple lines 

       ```
       ABABABBABCCCAAABBCACACAB
       BABABCBCBBCCBAAABA
       ABBABBABCACC
       ```

* `maxlength` is the maximum history length considered in causal states. 

  * ```matlab
    maxlength = 5; 
    ```
* `ofilename` CSSR-Matlab will rename files generated by CSSR. To understand what these output files represent,  please refer to the official CSSR depository <https://github.com/stites/CSSR#3-usage>. 

  * ```matlab
    % If using a relative path, files will be placed in the directory of the running script. 
    ofilename = 'cssr'; 
    % If using an absolute path, files will be placed in that directory. 
    ofilename = 'C:\Users\massw\Desktop\cssr'
    % Note that the current path is 'C:\Users\massw\Desktop' since 'cssr' is the output filename specified by the user.
    ```

* `siglevel` is the significance level, it should be in the range of (0,1). This parameter is 0.001 by default.

  * ```matlab
    siglevel = 0.001;
    ```

* `test_mode` is optional, `'ks'` by default. If `'ch'` flag is set , use Chi-Squared significance test to split causal states instead of the default Kolmogorov-Smirnov test. 

  * ```matlab
    test_mode = 'ks'; 
    test_mode = 'ch'; % NOT RECOMMENDED
    ```
***Note***: using 'ch' may lead Matlab to crash. This is a known bug in the function taken from *Numerical Recipes in C*. Try to evaluate the command again if failing.  

**Example usage :** 

```matlab
% add CSSR-Matlab to the path, "C:\Users\????\Documents\MATLAB\CSSR-Matlab" in my case.
if exist('runCSSR', 'file') ~= 2
    cssr_path = fullfile(userpath, 'CSSR-Matlab'); 
    addpath(cssr_path);
end 

% YOUR CODE HERE
% turn on multi-line mode automatically, significance level as 0.001, use 'ks' as test mode, and generate files named 'cssr' by default
out = runCSSR('ABC', {'ABABABCCCCBABABC'; 'ABABABCCCA'; 'BABBBACACACCB'}, 3);
```

The output `out` is a structure which includes fields `out.Cmu`, `out.state_prob` and `out.state_series`. 

* `out.Cmu` the statistical complexity. 
* `out.state_prob` the stationary probability distribution of each causal states. 
* `out.state_series` the series of causal states in the data. 

For details,  please refer to the official CSSR depository <https://github.com/stites/CSSR#3-usage>.



## CSSR-Matlab Installation 

To install, just download from Github. Then move all extracted files to the [default user work folder](https://au.mathworks.com/help/matlab/ref/userpath.html) and add th path of CSSR-Matlab. 

If you are familiar with git, you could alternatively clone CSSR-Matlab and install to the [default user work folder](https://au.mathworks.com/help/matlab/ref/userpath.html). 

Once all files are ready, run `makeCSSR.m` to compiles *mex* binaries if using CSSR-Matlab for the first time.


## CSSR Known Issues

Due to the limitations in the original C++ implementation, there are a few known issues in CSSR. For more details, see https://github.com/stites/CSSR#5-known-issues. 


## CSSR Parameters

One common question when using CSSR for the first time is how to choose the **history length** and **significance level**. 

To converge to a true epsilon machine, make sure the history length L is longer than true Markov order M (e.g. **L >= M**), otherwise, the number of states returned **may be less or higher than the actual one**. 

However, for a fixed-length N data stream, if keep increasing history length L further than the true Markov order of the given stationary stochastic process, **there is generally "blow up"** in the number of states.  

For more details, please see https://github.com/stites/CSSR#4-some-suggestions-about-parameters. 

## License

CSSR-Matlab is licensed as GNU General Public License version 2. 

CSSR v0.1.1 Copyright (C) 2002, Kristina Klinkner and Cosma Shalizi. 

All main changes to CSSR can be viewed in GitHub [commits](https://github.com/randoruf/CSSR-Matlab/commit/6063581b4946a48bad61c78c16f529bd2e5efda8).

## Acknowledgements

Although this is a small project, I still would like to say thanks to people in [tLab](https://sites.google.com/monash.edu/tlab/home). I also want to say thanks to [Cosma Shalizi](http://bactra.org/) and Kristina Klinkner, they purposed and developed the theory of CSSR. 

## Related Resources

* The original website of CSSR http://bactra.org/CSSR/
* The original GitHub repository of CSSR https://github.com/stites/CSSR
