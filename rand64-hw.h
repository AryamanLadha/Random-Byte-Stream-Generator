extern struct drand48_data buf;

/* Initialize the hardware rand64 implementation.  */
void
hardware_rand64_init (void);

/* Return a random value, using hardware operations.  */
unsigned long long
hardware_rand64 (void);

/* Finalize the hardware rand64 implementation.  */
void
hardware_rand64_fini (void);


/*Initialize the hardware mrand48_r implementation*/
void
hardware_mrand48_rng_init(void);

/*Return  a random value, using hardware operations*/
unsigned long long mrand48_rng(void);

/*Finalize the  mrand48_r implementation*/
void mrand48_rng_fini(void);