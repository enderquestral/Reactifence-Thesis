# Reactifence-Thesis
Written by Hannah Hellerstein for an undergraduate thesis at Reed College for the department of Computer Science. 
For the associated paper please see here: \[INSERT LINK HERE\].

(Code taken from @crozone's SpectrePoC, @CSL-KU's SpectreGuard, and @sebastien-riou's aes-brute-force)

Initial creation of a repo for a new Spectre variant 1 (CVE-2017-5753) defense that is a software-centric lfence replacement alternative. Implementation is inspired by SpectreGuard.

NOTE: At the time of writing, this thesis project is not entirely functional. It is a work in progress/open to edits. 


## The Logic behind this implementation

In effect, this implementation focuses around the theory that when addressing Spectre Variant 1, speed does not need to be entirely sacrificed to safety. Namely, some data is simply more important than other data, and thus we can indulge in preemptive execution on that code which we are willing to risk. If a data section is deemed important, then for safety's sake it makes sense to guard it with an lfence. If the data section/page in RAM is deemed completely unimportant, we can leak it (perhaps with some encoding) without much fear and have the speedup. Meanwhile, if the code is determined to be of Some importance, we're to ensure that the data is to be encoded and stored in a separate data structure (in an ideal modern computer this would be hardware inaccessable from the OS, but in this it'll likely be RAM) when being speculatively executed. 
Ideally, this separation of importances should lead to an amount of speedup present in Spectre-vulnerable code. It also should be patchable/have a flag-like structure so that it can be added onto other forms of older code.


## Results 

\[SECTION PENDING ON COMPLETED CODE\]

## Building

First, run the MakeFile in this repo. This is required for all functions to be available. 
If you wish to test with your own Spectre benchmark, make sure to include <reactifence.h> and if needed, have the flag REACTIFENCE_MITIGATION, as seen in spectre_reactifence. And that should be all that is reqired.
NOTE: This designed only for x86 machines.

## Executing

### Baseline example:
Once everything has been compiled, there are three options:

Run `./test_spectre` if you wish to see spectre running with no mitigations.  
Run `./test_spectre_lfence` if you wish to see spectre running against Intel's proposed strategy of using lfences.   
Run `./test_spectre_reactifence` if you wish to see this proposed defence alone. 


### On Synthetic Benchmarks:
(Note: Due to various reasons, this program has not been tested on the Spec2006 benchmarks as seen in the SpectreGuard repo.) 


## Citations

Yan, Mengjia. Mjyan0720/InvisiSpec-1.0. 2018. 2020. GitHub, https://github.com/mjyan0720/InvisiSpec-1.0.

Crosby, Ryan. Crozone/SpectrePoC. 2018. 2020. GitHub, https://github.com/crozone/SpectrePoC.

sebastien-riou. Sebastien-Riou/Aes-Brute-Force. 2017. 2021. GitHub, https://github.com/sebastien-riou/aes-brute-force.

CSL-KU/SpectreGuard. 2019. CSL-KU, 2020. GitHub, https://github.com/CSL-KU/SpectreGuard.

Meltdown and Spectre. https://spectreattack.com/. Accessed 23 Sept. 2020. https://spectreattack.com/spectre.pdf 
