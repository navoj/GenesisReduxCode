
  CONST
   pi  = 3.14159265;
   NRI = 1024;
   numGenMin = 2;
   bilateralSymmetry = 2;
   quadritlateralSymmetry = 4;
   cagannSize = 15;
   dgPopSize = 100;

  TYPE
   fullFileName = array [0..MAX_PATH-1] of char;

// -------------------------------------------------------
// Rietman, Edward (1994)
// Genesis Redux: Experiments Creating Artificial Life
// Windcrest (McGraw-Hill)
   chromosome64 = array [1..64] of smallint;

   chromosome9 = RECORD
    direction : smallint  (* Allowed values : -1, 1     *);
    x         : smallint  (* Screen x coordinate        *);
    y         : smallint  (* Screen y coordinate        *);
    newx      : smallint  (* New x value computed       *);
    newy      : smallint  (* New y value computed       *);
    height    : smallint  (* Initial height of organism *);
    angle     : smallint  (* Initial angle of organism  *);
    length    : smallint  (* Branch length change       *);
    blength   : smallint
   END;

   node_typ   = ^node_typ_r;
   node_typ_r = RECORD
    pgene   : chromosome9;
    nxt_lnk : node_typ
   END;

   organism = RECORD
    chromosome : chromosome64;
    fitval     : smallint;
    flag       : smallint;            // cagann1
    fitfract   : single;              // degaris1
    specieCode : smallint;            // degaris1
    mateFlag   : smallint             // degaris1
   END;

   organism_array = array[1..dgPopSize] of organism;                 // pp. 244 - 250
   organism_matrix = array[1..cagannSize,1..cagannSize] of organism; // pp. 232 - 244

   test_structure = array[1..8] of smallint;
   synaps_matrix  = array[1..8,1..8] of smallint;

// -------------------------------------------------------

   orgnsm_node_typ = ^orgnsm_node_typ_r;
   orgnsm_node_typ_r = RECORD
    genes : organism
   END;

   orgnsm_node_array = array[1..dgPopSize] of orgnsm_node_typ;
   orgnsm_node_matrix = array[1..cagannSize,1..cagannSize] of orgnsm_node_typ;   
   iXYZ_array = array[1..NRI] of smallint;   // Dawkins8
