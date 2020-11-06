/* 
 * File:   DefinitioTypes.h
 * Author: jcoelho
 *
 * Created on November 8, 2015, 10:41 PM
 */

#ifndef DEFINITIOTYPES_H
#define	DEFINITIOTYPES_H

/**
 * Type to represent the corners index, triangle index and vertex index. If the
 * undo/redo mechanism is used, the representation lose two bits.
 */
typedef unsigned int CornerType;

/**
 * Type to represent the level and sublevel of triangles and the level of the
 * vertices.
 */
typedef unsigned char LevelType;


#endif	/* DEFINITIOTYPES_H */
    