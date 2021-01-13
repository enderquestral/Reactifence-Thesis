# sglfence-Thesis
(Written by Hannah Hellerstein for an undergraduate thesis at Reed College for the department of Computer Science.)

(Code taken from @crozone's SpectrePoC, @CSL-KU's SpectreGuard, and @sebastien-riou's aes-brute-force)

Refrencing: https://spectreattack.com/spectre.pdf 
Initial creation of a repo for a new Spectre variant 1 (CVE-2017-5753) defense that is a software-centric lfence replacement alternative. Implementation is inspired by SpectreGuard

NOTE: At the time of writing, this thesis is not entirely functional. It is a work in progress. 

## The Logic behind this implementation

In effect, this implementation focuses around the theory that when addressing Spectre Variant 1, speed does not need to be entirely sacrificed to safety. Namely, some data is simply more important than other data, and thus we can indulge in preemptive execution on that code which we are willing to risk. If a data section is deemed important, then for safety's sake it makes sense to guard it with an lfence. If the data section/page in RAM is deemed completely unimportant, we can leak it (perhaps with some encoding) without much fear and have the speedup. Meanwhile, if the code is determined to be of Some importance, we're to ensure that the data is to be encoded and stored in a separate data structure (in an ideal modern computer this would be hardware inaccessable from the OS, but in this it'll likely be RAM) when being speculatively executed. 
Ideally, this separation of importances should lead to an amount of speedup present in Spectre-vulnerable code. It also should be patchable/have a flag-like structure so that it can be added onto other forms of older code.


## Results 

\[SECTION PENDING\]

## Building

First, run the MakeFile in this repo. This is required for all functions to be available. 
If you wish to test with your own Spectre benchmark, make sure to include <sglfence.h> and if needed, have the flag SGLFENCE_MITIGATION, as seen in spectre_sglfence . 

As for how to run it against Spectre Defences SpectreGuard and InvisiSpec (Thanks again @CSL-KU https://github.com/CSL-KU/SpectreGuard) then do the following:

\[SECTION PENDING\]

## Executing

### Baseline example:
Once everything has been compiled, there are three options:

Run `./test_spectre` if you wish to see spectre running with no mitigations.  
Run `./test_spectre_lfence` if you wish to see spectre running against Intel's proposed strategy of using lfences.   
Run `./test_spectre_sglfence` if you wish to see this proposed defence alone. 


### On Synthetic Benchmarks:
(Note: Due to various reasons, this program has not been tested on the Spec2006 benchmarks as seen in the SpectreGuard repo.) 

\[SECTION PENDING\]

