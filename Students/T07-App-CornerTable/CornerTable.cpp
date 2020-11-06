#include "CornerTable.h"
#include <cstdio>
#include <vector>
#include <list>
#include <cmath>

#include <iostream>
#include <stdlib.h>
using namespace std;



CornerTable::CornerTable( const CornerType* triangleList, double* vertexList,
                          const CornerType numberTriangles, const CornerType numberVertices,
                          const unsigned int numberCoordinatesByVertex )
{
    //Copy the counters to the Corner Table.
    _numberVertices = numberVertices;
    _numberTriangles = numberTriangles;
    _numberCoordinatesByVertex = numberCoordinatesByVertex;
    _maximumPoints = numberVertices;
    _maximumTriangles = numberTriangles;
    _reallocationFactor = 2;

    //Allocate the vectors.
    _cornerToVertex = std::vector<CornerType>( 3 * numberTriangles );
    _vertexToCorner = std::vector<CornerType>( numberVertices );
    _oppositeCorner = std::vector<CornerType>( 3 * numberTriangles );
    _attributes = std::vector<double>( numberCoordinatesByVertex * numberVertices );

    //Coppy the informations.
    memcpy( &_cornerToVertex[0], triangleList, 3 * numberTriangles * sizeof (CornerType ) );
    memcpy( &_attributes[0], vertexList, numberCoordinatesByVertex * numberVertices * sizeof ( double ) );

    //Initialize vectors
    memset( &_vertexToCorner[0], 0, numberVertices * sizeof (CornerType ) );
    for (CornerType i = 0; i < 3 * _numberTriangles; i++)
    {
        _oppositeCorner[i] = BORDER_CORNER;
    }

    //Build the opposite table.
    buildOppositeTable( );

}



CornerTable::~CornerTable( )
{
}



unsigned int CornerTable::getNumberAttributesByVertex( ) const
{
    return _numberCoordinatesByVertex;
}



CornerType CornerTable::getNumberVertices( ) const
{
    return _numberVertices;
}



CornerType CornerTable::getNumTriangles( ) const
{
    return _numberTriangles;
}



double* CornerTable::getAttributes( ) const
{
    return ( double* ) ( &_attributes[0] );
}



CornerType CornerTable::cornerToVertexIndex( const CornerType corner ) const
{
    return _cornerToVertex[corner];
}



CornerType CornerTable::vertexToCornerIndex( const CornerType vertex ) const
{
    return _vertexToCorner[ vertex ];
}



const CornerType* CornerTable::getTriangleList( ) const
{
    return &_cornerToVertex[0];
}



void CornerTable::setReallocationFactor( const unsigned int reallocationFactor )
{
    if (reallocationFactor > 1)
    {
        _reallocationFactor = reallocationFactor;
    }
}



unsigned int CornerTable::getReallocationFactor( ) const
{
    return _reallocationFactor;
}



bool CornerTable::edgeFlip( const CornerType corner )
{
    if (corner == BORDER_CORNER || _oppositeCorner[corner] == BORDER_CORNER)
    {
        //It is not allowed to flip.
        return false;
    }

    //Identify the incidences.
    CornerType c1 = cornerNext( corner );
    CornerType c2 = cornerPrevious( corner );
    CornerType c3 = _oppositeCorner[corner];
    CornerType c4 = cornerNext( c3 );
    CornerType c5 = cornerPrevious( c3 );
    CornerType b = _oppositeCorner[c1];
    CornerType c = _oppositeCorner[c4];

    //Get the vertices corner.
    CornerType t = _cornerToVertex[corner];
    CornerType u = _cornerToVertex[c2];
    CornerType v = _cornerToVertex[c1];
    CornerType s = _cornerToVertex[c3];

    //Change the triangulation.
    _cornerToVertex[c5] = t;
    _cornerToVertex[c2] = s;

    //Save the corners to vertex.
    _vertexToCorner[t] = corner;
    _vertexToCorner[v] = c1;
    _vertexToCorner[u] = c4;
    _vertexToCorner[s] = c2;

    //Ajust the opposite corners.
    _oppositeCorner[c4] = c1;
    _oppositeCorner[c1] = c4;

    _oppositeCorner[c3] = b;
    if (b != BORDER_CORNER)
    {
        _oppositeCorner[b] = c3;
    }

    _oppositeCorner[corner] = c;
    if (c != BORDER_CORNER)
    {
        _oppositeCorner[c] = corner;
    }
    return true;
}



bool CornerTable::edgeUnflip( const CornerType corner )
{
    if (corner < 0 || _oppositeCorner[corner] == BORDER_CORNER)
    {
        //It is not allowed to flip.
        return false;
    }

    //Identify the incidences.
    CornerType c2 = cornerNext( corner );
    CornerType c0 = cornerPrevious( corner );
    CornerType c4 = _oppositeCorner[corner];
    CornerType c5 = cornerNext( c4 );
    CornerType c3 = cornerPrevious( c4 );
    CornerType b = _oppositeCorner[c3];
    CornerType c = _oppositeCorner[c0];

    //Get the vertices corner.
    CornerType v = _cornerToVertex[corner];
    CornerType u = _cornerToVertex[c4];
    CornerType s = _cornerToVertex[c2];
    CornerType t = _cornerToVertex[c0];

    //Change the triangulation.
    _cornerToVertex[c5] = v;
    _cornerToVertex[c2] = u;

    //Save the corners to vertex.
    _vertexToCorner[s] = c3;
    _vertexToCorner[u] = c2;
    _vertexToCorner[v] = corner;
    _vertexToCorner[t] = c0;

    //Ajust the opposite corners.
    _oppositeCorner[c0] = c3;
    _oppositeCorner[c3] = c0;
    _oppositeCorner[corner] = b;
    if (b != BORDER_CORNER)
    {
        _oppositeCorner[b] = corner;
    }

    _oppositeCorner[c4] = c;
    if (c != BORDER_CORNER)
    {
        _oppositeCorner[c] = c4;
    }
    return true;
}



void CornerTable::edgeSplit( const CornerType corner, const double* coordinates )
{
    if (corner < 0 || corner == BORDER_CORNER)
    {
        return;
    }

    //Resize the vectors if it is necessary.
    resizeVectors( );

    //Identify the incidences.
    CornerType c2 = cornerPrevious( corner );
    CornerType c1 = cornerNext( corner );
    CornerType c3 = _oppositeCorner[ corner ];
    CornerType c4 = BORDER_CORNER;
    CornerType c5 = BORDER_CORNER;
    CornerType d = _oppositeCorner[c2];
    CornerType a = BORDER_CORNER;

    //Get the vertices corner.
    CornerType t = _cornerToVertex[corner];
    CornerType v = _cornerToVertex[c1];
    CornerType u = _cornerToVertex[c2];

    //Identify the incidences.
    if (c3 != BORDER_CORNER)
    {
        c4 = cornerNext( c3 );
        c5 = cornerPrevious( c3 );
        a = _oppositeCorner[c5];
    }

    //Get the index of the new vertex.
    CornerType indexNewPoint = _numberVertices;

    //Copy the vertex coordinates to attributes vector.
    for (unsigned int i = 0; i < _numberCoordinatesByVertex; i++)
    {
        _attributes[_numberCoordinatesByVertex * indexNewPoint + i ] = coordinates[i];
    }
    _numberVertices++;

    //Get indexes of the new triangles.
    CornerType triangleAIndex = _numberTriangles;
    CornerType triangleBIndex = _numberTriangles + 1;

    //Save corners to vertex.
    _vertexToCorner[indexNewPoint] = 3 * triangleAIndex + 2;
    _vertexToCorner[t] = 3 * triangleAIndex;
    _vertexToCorner[v] = 3 * triangleAIndex + 1;

    //Add the new triangles on the final of the list.
    _cornerToVertex[3 * triangleAIndex + 0] = t;
    _cornerToVertex[3 * triangleAIndex + 1] = v;
    _cornerToVertex[3 * triangleAIndex + 2] = indexNewPoint;

    //Increment the number of triangles.
    _numberTriangles++;

    if (c3 != BORDER_CORNER)
    {
        _cornerToVertex[3 * triangleBIndex] = _cornerToVertex[c3];
        _cornerToVertex[3 * triangleBIndex + 1] = u;
        _cornerToVertex[3 * triangleBIndex + 2] = indexNewPoint;
        _numberTriangles++;

        _vertexToCorner[_cornerToVertex[c3]] = 3 * triangleBIndex;
        _vertexToCorner[u] = 3 * triangleBIndex + 1;
    }

    //Change the first triangulations.
    _cornerToVertex[c1] = indexNewPoint;
    if (c3 != BORDER_CORNER)
    {
        _cornerToVertex[c4] = indexNewPoint;
    }

    //Update the opposite table.
    _oppositeCorner[c2] = 3 * triangleAIndex + 1;
    _oppositeCorner[3 * triangleAIndex + 1] = c2;

    if (d != BORDER_CORNER)
    {
        _oppositeCorner[d] = 3 * triangleAIndex + 2;
    }
    _oppositeCorner[3 * triangleAIndex + 2] = d;

    if (c3 != BORDER_CORNER)
    {
        _oppositeCorner[3 * triangleAIndex] = c3;
        _oppositeCorner[c3] = 3 * triangleAIndex;
        _oppositeCorner[cornerPrevious( c3 )] = 3 * triangleBIndex + 1;
        _oppositeCorner[ 3 * triangleBIndex + 1] = c5;
        _oppositeCorner[3 * triangleBIndex + 2] = a;
        _oppositeCorner[3 * triangleBIndex] = corner;
        _oppositeCorner[corner] = 3 * triangleBIndex;

    }
    else
    {
        _oppositeCorner[3 * triangleAIndex] = BORDER_CORNER;
    }

    if (a != BORDER_CORNER && c3 != BORDER_CORNER)
    {
        _oppositeCorner[a] = 3 * triangleBIndex + 2;
    }
}



void CornerTable::edgeWeld( const CornerType corner )
{
    //Identify the incidences.
    CornerType c2 = cornerNext( corner );
    CornerType c0 = cornerPrevious( corner );
    CornerType c7 = _oppositeCorner[c2];
    CornerType c3 = BORDER_CORNER;
    CornerType c4 = BORDER_CORNER;
    CornerType c5 = BORDER_CORNER;
    CornerType a = BORDER_CORNER;
    CornerType d = BORDER_CORNER;
    if (c7 != BORDER_CORNER)
    {
        c3 = _oppositeCorner[cornerPrevious( c7 )];
        if (c3 != BORDER_CORNER)
        {
            c4 = cornerNext( c3 );
            c5 = cornerPrevious( c3 );
            if (_oppositeCorner[c5] != BORDER_CORNER)
            {
                a = _oppositeCorner[ cornerNext( _oppositeCorner[c5] ) ];
            }
        }
        d = _oppositeCorner[ cornerNext( c7 ) ];
    }

    if (c4 != BORDER_CORNER)
    {
        CornerType u = _cornerToVertex[c2];
        _cornerToVertex[c4] = u;
        if (_oppositeCorner[c5] != BORDER_CORNER)
        {
            _cornerToVertex[cornerNext( _oppositeCorner[c5] )] = u;
            _cornerToVertex[cornerPrevious( _oppositeCorner[c5] )] = u;
            _numberTriangles--;
        }

    }

    //Remove the vertex.
    _cornerToVertex[corner] = _cornerToVertex[c7 ];
    _numberVertices--;
    if (c7 != BORDER_CORNER)
    {
        _cornerToVertex[cornerNext( c7 )] = _cornerToVertex[ c7 ];
        _cornerToVertex[cornerPrevious( c7 )] = _cornerToVertex[ c7 ];
        _numberTriangles--;
    }

    //Free the triangles.
    if (_oppositeCorner[c0] != BORDER_CORNER)
    {
        _oppositeCorner[_oppositeCorner[c0]] = BORDER_CORNER;
        _oppositeCorner[cornerNext( _oppositeCorner[c0] )] = BORDER_CORNER;
        _oppositeCorner[cornerPrevious( _oppositeCorner[c0] )] = BORDER_CORNER;
    }
    if (c7 != BORDER_CORNER)
    {
        _oppositeCorner[c7] = BORDER_CORNER;
        _oppositeCorner[cornerNext( c7 )] = BORDER_CORNER;
        _oppositeCorner[cornerPrevious( c7 )] = BORDER_CORNER;
    }

    //Update the opposite table.
    _oppositeCorner[c2] = d;
    if (d != BORDER_CORNER)
    {
        _oppositeCorner[d] = c2;
    }

    if (c5 != BORDER_CORNER)
        _oppositeCorner[c5] = a;

    if (a != BORDER_CORNER)
    {
        _oppositeCorner[a] = c5;
    }
    _oppositeCorner[c0] = c3;
    if (c3 != BORDER_CORNER)
    {
        _oppositeCorner[c3] = c0;
    }

    //Update the vector with a corner to each vertex.
    CornerType t = _cornerToVertex[corner];
    CornerType u = _cornerToVertex[cornerPrevious( corner )];
    CornerType v = _cornerToVertex[cornerNext( corner )];
    _vertexToCorner[t] = corner;
    _vertexToCorner[u] = cornerPrevious( corner );
    _vertexToCorner[v] = cornerNext( corner );
    if (_oppositeCorner[corner] != BORDER_CORNER)
    {
        CornerType s = _cornerToVertex[corner];
        _vertexToCorner[s] = _oppositeCorner[corner];
    }
}



void CornerTable::buildOppositeTable( )
{
    //The adjacency list from corner to vetices.
    std::vector< std::list< CornerType > > cornersInVertices;
    //Allocate memory for the adjacency list.
    cornersInVertices.resize( _numberVertices );
    for (CornerType corner = 0, totalCorners = 3 * _numberTriangles; corner < totalCorners; corner++)
    {
        //Get the vertex of the corner.
        CornerType vertex = _cornerToVertex[corner];

        //Add the corner at the vertex list.
        cornersInVertices[vertex].push_back( corner );

        //Update the vector that store a corner to each vertex.
        _vertexToCorner[vertex] = corner;
    }


    //Compute the opposite corner to each corner.
    for (CornerType i = 0; i < _numberTriangles; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            CornerType corner = 3 * i + j;

            //Verifica se o oposto do corner eh BORDER_CORNER. Se nao for e o caso de ja
            //ter calculado seu corner oposto
            if (_oppositeCorner[corner] == BORDER_CORNER)
            {
                //Compute opposite corner analyzing the opposite edge.
                CornerType vertexNext = _cornerToVertex[cornerNext( corner )];
                CornerType vertexPrevious = _cornerToVertex[cornerPrevious( corner )];

                //Initialize the opposite edge.
                CornerType opposite = BORDER_CORNER;

                //Search by the opposite corner.
                for (std::list<CornerType>::iterator it =
                     cornersInVertices[vertexPrevious].begin( ); it != cornersInVertices[vertexPrevious].end( ); ++it)
                {
                    //Get one of the corners of the vertice.
                    CornerType cornerOfVertexPrevious = *it;

                    //Get the next corner.
                    CornerType nextCorner = cornerNext( cornerOfVertexPrevious );

                    //Verify if find out the opposite face.
                    if (_cornerToVertex[nextCorner] == vertexNext)
                    {
                        //get the opposite corner.
                        opposite = cornerPrevious( cornerOfVertexPrevious );
                        break;
                    }
                }

                //If the opposite corner is BORDER_CORNER, then the opposite
                //edge is a border edge.
                if (opposite == BORDER_CORNER)
                {
                    //Update the table that stores a corner to each vextex such
                    //that to turn each traverse to the border.
                    _vertexToCorner[vertexNext] = cornerNext( corner );
                    continue;
                }

                //Update the opposite corner.
                _oppositeCorner[corner] = opposite;
                _oppositeCorner[opposite] = corner;
            }
        }
    }
}



void CornerTable::resizeVectors( )
{
    //Verify if it is necessary to allocate more space.
    if (_numberTriangles + 1 >= _maximumTriangles)
    {
        //Allocate more memory for triangulation.
        _cornerToVertex.resize( _reallocationFactor * 3 * _maximumTriangles );

        //Allocate more memory for the opposite table.
        _oppositeCorner.resize( _reallocationFactor * 3 * _maximumTriangles );

        //Update the size of current storage allocated for triangle list.
        _maximumTriangles *= _reallocationFactor;
    }

    if (_numberVertices == _maximumPoints)
    {
        //Allocate more memory for the vertex list.
        _attributes.resize( _reallocationFactor * _numberCoordinatesByVertex * _numberVertices );

        //Allocate more memory for the the vector that stores a corner to each
        //vertex.
        _vertexToCorner.resize( _reallocationFactor * _numberVertices );

        //Update the size of current storage allocated for vertex list.
        _maximumPoints *= _reallocationFactor;
    }
}



const std::vector<CornerType> CornerTable::getCornerNeighbours( const CornerType corner ) const
{
    //Vector to stores the neighbor corner.
    std::vector<CornerType> neighboursCorners;

    //Get the firt corner on the list.
    CornerType firstCorner = cornerNext( corner );

    //Run the loop until find the border or the first corner.
    CornerType currentCorner = firstCorner;
    do
    {
        //Add the current corner on list.
        neighboursCorners.push_back( currentCorner );

        //Get the next corner on right direction.
        currentCorner = cornerRight( currentCorner );
    }
    while (currentCorner != BORDER_CORNER && currentCorner != firstCorner);

    // If the search stopped on the first corner return the list.
    if (currentCorner != BORDER_CORNER)
        return neighboursCorners;

    //Otherwise, continue on the left direction.
    currentCorner = cornerPrevious( corner );
    do
    {
        //Add the current corner on list.
        neighboursCorners.push_back( currentCorner );

        //Get the next corner on left direction.
        currentCorner = cornerLeft( currentCorner );
    }
    while (currentCorner != BORDER_CORNER);

    //Return the neighbor list.
    return neighboursCorners;
}



CornerType CornerTable::computeEulerCharacteristic( )
{
    //Variaveis para contar numero de arestas da malha.
    CornerType numberBorderEdges = 0, numberInsideEdges = 0;

    //Compute the number of edges on surface.
    for (CornerType i = 0; i < 3 * _numberTriangles; i++)
    {
        if (_oppositeCorner[i] != BORDER_CORNER)
        {
            numberInsideEdges++;
        }
        else
        {
            numberBorderEdges++;
        }
    }

    //Compute the number of edges on surface.
    CornerType numberEdges = numberInsideEdges / 2 + numberBorderEdges;

    //Retorna a caracteristica de Euler.
    return _numberVertices - numberEdges + _numberTriangles;
}



void CornerTable::printTriangleList( )
{
    for (CornerType i = 0; i < _numberTriangles; i++)
    {
        printf( "%u: (%u, %u, %u)\n", i, ( unsigned int ) _cornerToVertex[3 * i + 0],
                ( unsigned int ) _cornerToVertex[3 * i + 1], ( unsigned int ) _cornerToVertex[3 * i + 2] );
    }
    printf( "\n\n" );
}
