#ifndef DEBUGLIB_H
#define DEBUGLIB_H

#define debug0(x); if(DEBUG){printf(x "\n");}
#define debug1(x, y); if(DEBUG){printf(x ", " #y ": %2d\n", y);}
#define debug2(x, y, z); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d\n", y, z);}
#define debug3(x, y, z, w); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d\n", y, z, w);}
#define debug4(x, y, z, w, v); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d\n", y, z, w, v);}
#define debug5(x, y, z, w, v, u); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d, " #u ": %2d\n", y, z, w, v, u);}
#define debug6(x, y, z, w, v, u, t); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d, " #u ": %2d, " #t ": %2d\n", y, z, w, v, u, t);}
#define debug7(x, y, z, w, v, u, t, s); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d, " #u ": %2d, " #t ": %2d, " #s ": %2d\n", y, z, w, v, u, t, s);}
#define debug8(x, y, z, w, v, u, t, s, r); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d, " #u ": %2d, " #t ": %2d, " #s ": %2d, " #r ": %2d\n", y, z, w, v, u, t, s, r);}
#define debug9(x, y, z, w, v, u, t, s, r, q); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d, " #u ": %2d, " #t ": %2d, " #s ": %2d, " #r ": %2d, " #q ": %2d\n", y, z, w, v, u, t, s, r, q);}
#define debug10(x, y, z, w, v, u, t, s, r, q, p); if(DEBUG){printf(x ", " #y ": %2d, " #z ": %2d, " #w ": %2d, " #v ": %2d, " #u ": %2d, " #t ": %2d, " #s ": %2d, " #r ": %2d, " #q ": %2d, " #p ": %2d\n", y, z, w, v, u, t, s, r, q, p);}

#define DEBUG false

#endif