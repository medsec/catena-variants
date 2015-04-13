Catena-Variants
================
This is a flexible C++-port of the Catena password scrambler. In contrast
to the reference implementation, it allows easy extension and modification of
Catena's internal parts. This flexibility comes with the downside of
significant higher memory consumption compared to the reference implementation.
Note that this framework is supposed to be used for testing. For productive 
environments, we recommend to use the reference implementation.

Reference implementation:
https://github.com/medsec/catena

Academic paper:
<a href="http://www.uni-weimar.de/fileadmin/user/fak/medien/professuren/Mediensicherheit/Research/Publications/catena-v3.1.pdf">catena-v3.1.pdf</a>


Applications
-------
* catena-scramble : A simple CLI that computes the hash for a given set of 
	parameters
* catena-measure : A benchmarking tool to test a chosen set of parameters 


Functions
---------
Algorithms (Flap function):
* The default algorithm of Catena-Butterfly and -Dragonfly

Hashes (H or H' function):
* Blake2b
* Blake2b1, which is a single round of Blake2b
* SHA512
* Galois-field multiplication reduced by the GCM polynomial
* A multiplication-based hash function implemented by Bill Cox 

RandomLayers (Gamma function):
* The random layer (Gamma) of Catena-Butterfly and -Dragonfly
* A dummy layer that can be used to skip this step 

Graphs (F function):
* Double-Butterfly
* Bit-reversal
* A combination of bit-reversal with ideas from Gray-code as suggested by Ben Harris


Using Catena-Variants in your own applications
-----------------------------------------------
We recommend to instantiate a Catena_Variants::Catena Object from 
Catena_Variants::CatenaFactory::instance().create(...)


Compatibility
-------------
Catena-Variants should work on all 32-bit and 64-bit little-endian systems. All 
recent versions of clang and gcc can be used to compile. Some functions may 
also work on big-endian systems.


Dependencies
------------
* clang(++)		(http://clang.llvm.org/)
* openssl       (http://www.openssl.org/)
* make 			(http://www.gnu.org/software/make/)


How to extend Catena-Variants
==============================
Catena-Variants makes use of the factory pattern with class registration. 
In most cases, this allows you to add your own functionality by simply placing
a header file(`.hpp`) and the source code in a `.cpp` file into the corresponding
directories. To register your class simply include `registry.hpp` from the `src/`
dir and add the following line to your `.cpp` file:

    Registry<YOURCLASS> SOME_UNIQUE_NAME;

The constructor of YOURCLASS should initialize `Registerable` as follows:

    Registerable("My name", "My short handle", "My description")
	

Algorithms
----------
Algorithms must be derived from
    
    Algorithm<YOURCLASS>

which can be found in `algorithm.hpp`. YOURCLASS must provide the following method:

    void flap(const uint8_t x[H_LEN], const uint8_t lambda, 
		const uint8_t garlic, const uint8_t *salt, const uint8_t saltlen, 
		uint8_t h[H_LEN]);

Graphs
------
Graphs must be derived from
    
    Graph<YOURCLASS>

which can be found in `graph.hpp`. YOURCLASS must provide the following methods:

    void process(const uint8_t x[H_LEN], const uint8_t lambda, 
                    const uint8_t garlic, const uint8_t *salt, 
                    const uint8_t saltlen, uint8_t *r, uint8_t h[H_LEN]);

    uint64_t getMemoryRequirement(uint8_t garlic)const;
    
    uint8_t getDefaultLambda()const;
    uint8_t getDefaultGarlic()const;
    uint8_t getDefaulMinGarlic()const;
    const uint8_t* getDefaultVersionID()const;

Fast Hashes
-----------
Fast hashes must be derived from
    
    HashFast<YOURCLASS>

which can be found in `hashfast.hpp`. YOURCLASS must provide the following methods:

    /* Reduced Hash function. The 2 Inputs are expected to be of size H_LEN
    */
    void Hash(int vindex, const uint8_t* i1, 
            const uint8_t* i2, uint8_t hash[H_LEN]);
    /* Resets the State used for HashFast if necessary
    */
    void ResetState();

Full Hashes
-----------
Full hashes must be derived from
    
    HashFull<YOURCLASS>

which can be found in `hashfull.hpp`. YOURCLASS must provide the following methods:

    void Hash1(const uint8_t *input, const uint32_t inputlen,
                uint8_t hash[H_LEN])const;

    void Hash2(const uint8_t *i1, const uint8_t i1len,
                const uint8_t *i2, const uint8_t i2len,
                uint8_t hash[H_LEN])const;

    void Hash3(const uint8_t *i1, const uint8_t i1len,
                const uint8_t *i2, const uint8_t i2len,
                const uint8_t *i3, const uint8_t i3len,
                uint8_t hash[H_LEN])const;

    void Hash4(const uint8_t *i1, const uint8_t i1len,
                const uint8_t *i2, const uint8_t i2len,
                const uint8_t *i3, const uint8_t i3len,
                const uint8_t *i4, const uint8_t i4len,
                uint8_t hash[H_LEN])const;

    void Hash5(const uint8_t *i1, const uint8_t i1len,
                const uint8_t *i2, const uint8_t i2len,
                const uint8_t *i3, const uint8_t i3len,
                const uint8_t *i4, const uint8_t i4len,
                const uint8_t *i5, const uint8_t i5len,
                uint8_t hash[H_LEN])const;

Random Layers
-------------
Random layers must be derived from
    
    RandomLayer<YOURCLASS>

which can be found in `randomlayer.hpp`. YOURCLASS must provide the following methods:

    virtual void process(const uint8_t x[H_LEN], const uint8_t lambda, 
                    const uint8_t garlic, const uint8_t *salt, 
                    const uint8_t saltlen, uint8_t *r);

Tips
----
* Catena-Variants makes use of the copy constructor to create objects. If
  the implicit copy constructor will not work for your added classes, you 
  must implement custom copy constructors for your classes.
* If you do not supply a new unique Version_ID to the instance of Catena, the 
  default Version_ID of the graph is used.