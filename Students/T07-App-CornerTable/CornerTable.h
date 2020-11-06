#ifndef _CORNER_TABLE_
#define _CORNER_TABLE_

#include <vector>
#include <cstring>
#include "DefinitionTypes.h"


/**@class CornerTable
 * Topological data structure to store a triangle mesh and to allow perform
 * necessary topological operations.
 */
class CornerTable
{
public:
    /**
     * Constructor to mount the Corner Table topological data structure. This
     * operation's complexity is O(m + n), where m is the number of edges and
     * n the number of vertices. Furthemore, all vectores are copied to the 
     * data structure.
     * @param triangleList - the oriented triangle list.
     * @param vertexList - vertex list. It is assumed that the vertices 
     * information are contiguously stored. For exemplo, if each vertex has the
     * coordinates (x, y, z); the list will be of the form: xyzxyzxyzxyz...
     * @param numberTriangles - number of triangles on surface..
     * @param numberVertices - number of vertices on surface.
     * @param numberCoordinatesPerVertex - number of coordinates by vertex. It
     * turns possible to store any vertex property in the vertex list. For example,
     * the vertex list could be of the form: XYZNxNyNzXYZNxNyNz... in order to
     * stored the vertex normal together with its coordinates. In this case, this
     * parameter will be 6 instead of 3.
     */
    CornerTable( const CornerType *triangleList, double *vertexList, const CornerType numberTriangles,
        const CornerType numberVertices, const unsigned int numberCoordinatesByVertex );

    /**
     * Destructor of the Corner Table. Free all allocated memory.
     */
    ~CornerTable( );

    /**
     * Return the number of vertices on current surface.
     * @return - number of vertices on current surface.
     */
    CornerType getNumberVertices( ) const;

    /**
     * Return the number of triangle on current surface.
     * @return  - number of triangle on current surface.
     */
    CornerType getNumTriangles( ) const;

    /**
     * Return the vertex list with the attributes of each vertex.
     * @return - vertex list with the attributes of each vertex.
     */
    double* getAttributes( ) const;

    /**
     * Return the vertex of the corner. It is obtained by acessing the triangle
     * list on the position corner.
     * @param corner - corner to get the vertex.
     * @return - vertex of the corner.
     */
    CornerType cornerToVertexIndex( const CornerType corner ) const;

    /**
     * Return a corner of the vertex. This operation is not necessary to
     * refinement algorithms.
     * @param vertex - vertex index.
     * @return - a corner of the vertex.
     */
    CornerType vertexToCornerIndex( const CornerType vertex ) const;

    /**
     * Return the triangle list vector.
     * @return - triangle list.
     */
    const CornerType * getTriangleList( ) const;

    /**
     * Return the triangle of the corner.
     * @param corner - corner index.
     * @return - triangle of the corner.
     */
    inline CornerType cornerTriangle( const CornerType corner ) const
    {
        return corner / 3;
    };

    /**
     * Return the next corner inside of the triangle following the triangle 
     * orientation.
     * @param corner - corner index.
     * @return - next corner inside of the triangle.
     */
    inline CornerType cornerNext( const CornerType corner ) const
    {
        return 3 * ( corner / 3 ) + ( corner + 1 ) % 3;
    };

    /**
     * Return the previous corner inside of the triangle following the triangle 
     * orientation.
     * @param corner - corner index.
     * @return - previous corner inside of the triangle.
     */
    inline CornerType cornerPrevious( const CornerType corner ) const
    {
        return 3 * ( corner / 3 ) + ( corner + 2 ) % 3;
    };

    /**
     * Return the left corner. This means the opposite of the previous corner.
     * @return the left corner. It must to test the returned value. As the opposite
     * vector is used, the returned value can be CornerTable::BORDER_CORNER in
     * the of the surface border.
     */
    inline CornerType cornerLeft( const CornerType corner ) const
    {
        return _oppositeCorner[cornerPrevious( corner )];
    };

   /**
     * Return the right corner. This means the opposite of the next corner.
    * @param corner - corner index.
     * @return the right corner. It must to test the returned value. As the opposite
     * vector is used, the returned value can be CornerTable::BORDER_CORNER in
     * the of the surface border.
     */
    inline CornerType cornerRight( const CornerType corner ) const
    {
        return _oppositeCorner[cornerNext( corner )];
    };

    /**
     * Return the opposite corner.
     * @param corner - corner index.
     * @return - the opposite corner. It must to test the returned value. As the opposite
     * vector is used, the returned value can be CornerTable::BORDER_CORNER in
     * the of the surface border.
     */
    inline CornerType cornerOpposite( const CornerType corner ) const
    {
        return _oppositeCorner[corner];
    };

    /**
     * Return the swing corner. This is the next corner on vertex star following
     * the right direction.
     * @param corner - corner index.
     * @return - swing corner. It must to test the returned value. As the opposite
     * vector is used, the returned value can be CornerTable::BORDER_CORNER in
     * the of the surface border.
     */
    inline CornerType cornerSwing( const CornerType corner ) const
    {
        CornerType r = cornerRight( corner );
        if ( r != CornerTable::BORDER_CORNER )
        {
            return cornerNext( r );
        }
        return r;
    };

    /**
     * Return the unswing corner. This is the previous corner on vertex star following
     * the left direction.
     * @param corner - corner index.
     * @return - unswing corner. It must to test the returned value. As the opposite
     * vector is used, the returned value can be CornerTable::BORDER_CORNER in
     * the of the surface border.
     */
    inline CornerType cornerUnswing( const CornerType corner ) const
    {
        CornerType l = cornerLeft( corner );
        if ( l != CornerTable::BORDER_CORNER )
        {
            return cornerPrevious( l );
        }
        return l;
    };

    /**
     * Return the reallocation factor. This is used to reallocate the vector
     * when they are full. By default this value equal 2. This means that when
     * it is necessary, each vector is doubled. The new reallocation factor must
     * be greater than 1.
     * @return - current reallocation factor.
     */
    unsigned int getReallocationFactor( ) const;

    /**
     * Define a new reallocation factor. This is used to reallocate the vector
     * when they are full. By default this value equal 2. This means that when
     * it is necessary, each vector is doubled. The new reallocation factor must
     * be greater than 1.
     * @param realocationFactor - new reallocation factor value.
     */
    void setReallocationFactor( const unsigned int realocationFactor );

    /**
     * Perform the Edge Split operation on the opposite edge. This operation 
     * inserts a new vertex over the edge opposite to the 'corner' and replace
     * two triangles by four.
     * @param corner - corner index opposite to the Edge to apply the Edge Split
     * Operation.
     * @param coordinates - new coordinates and attributes to the new vertex.
     */
    void edgeSplit( const CornerType corner, const double* coordinates );

    /**
     * Perform the Edge Flip operation on the edge opposite to the 'corner'. In
     * case of a border edge this operation is not allowed.
     * @param corner - corner index opposite to the Edge to apply the Edge Flip
     * Operation.
     * @return - true if the operation is allowed and false otherwise.
     */
    bool edgeFlip( const CornerType corner );

    /**
     * Perform the Edge Unflip operation on the edge opposite to the 'corner'
     * in order to revert the modifications of the Edge Flip operation.
     * @param corner - corner index opposite to the Edge to apply the Edge Unflip
     * Operation. This corner will always be c, to revert the Edge Flip operation
     * on corner c.
     * @return - true if the operation is allowed and false otherwise.
     */
    bool edgeUnflip( const CornerType corner );

    /**
     * Remove a vertex with valence 4 to revert the Edge Split operation.
     * @param corner - corner index of the vertex to be removed. This corner
     * will always be next(c) to revert the Edge Split operation on corner c.
     */
    void edgeWeld( const CornerType corner );

    /**
     * Return the number of attributes by vertex.
     * @return - number of attributes by vertex.
     */
    unsigned int getNumberAttributesByVertex( ) const;

    /**
     * Print the triangle list. Used just in debug.
     */
    void printTriangleList( );

    /**
     * Compute the neighbors corners on the vertex star of the 'corner'.
     * @param corner - corner index of a vertex.
     * @return - vector with neighbors.
     */
    const std::vector<CornerType> getCornerNeighbours( const CornerType corner ) const;

    /**
     * Compute the Euler Characteristic. Used just in debug.
     * @return - Euler Characteristic.
     */
    CornerType computeEulerCharacteristic( );

    /**
     * Especial id for birder edge.
     */
    static const CornerType BORDER_CORNER = -1;
private:
    /**
     * The opposite corners vector, or table O.
     */
    std::vector< CornerType > _oppositeCorner;

    /**
     * The triangle list vector, or table V.
     */
    std::vector< CornerType > _cornerToVertex;

    /**
     * Store a corner to each vertex. This vector is not necessary to refinement
     * algorithms.
     */
    std::vector< CornerType > _vertexToCorner;

    /**
     * The attributes vertex vector, or table G (geometry table).
     */
    std::vector<double> _attributes;

    /**
     * Number of coordinates by vertex.
     */
    unsigned int _numberCoordinatesByVertex;

    /**
     * Number of vertex on current surface.
     */
    CornerType _numberVertices;

    /**
     * Number of triangles on current surface.
     */
    CornerType _numberTriangles;

    /**
     * The size of the storage space currently allocated on geometry table.
     */
    CornerType _maximumPoints;

    /**
     * The size of the storage space currently allocated on triangle list table.
     */
    CornerType _maximumTriangles;

    /**
     * The reallocation factor.
     */
    unsigned int _reallocationFactor;
private:
    /**
     * Reallocate memory for vectors when it is necessary.
     */
    void resizeVectors( );

    /**
     * Build the opposite table on constructor.
     */
    void buildOppositeTable( );
};

#endif
