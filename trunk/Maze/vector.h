#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"
#include "math.h"

/* Vector
 *
 * A mathematical, N-Dimensional vector of undefined type
 */

template<typename Type, uint Dimension> class Vector
{
protected:
  Type v[Dimension];
public:
  Vector()
  { 
    for(uint c = 0; c < Dimension; c++)
      v[c] = (Type)0;
  }
  Type getLength() const
  {
    Type currentLength = 0;
    for(uint c = 0; c < Dimension; c++)
      currentLength += v[c]*v[c];
    return sqrt(currentLength);
  }
  void setLength(Type length)
  { 
    //Get multiplication factor
    Type factor = length / getLength();
    
    //Apply factor to all elements
    for(uint c = 0; c < Dimension; c++)
      v[c] *= factor;
  }

  Type& operator[](uint8 element) { return v[element]; }  

  // mathematical operators

  Vector<Type, Dimension> operator*(Type scalar)
  {
    Vector<Type, Dimension> r;
    for(uint c = 0; c < Dimension; c++)
      r[c] = v[c] * scalar;
    return r;
  }

  Vector<Type, Dimension> operator+(Vector<Type, Dimension>& vArg)
  {
    Vector<Type, Dimension> r;
    for(uint c = 0; c < Dimension; c++)
      r[c] = v[c] + vArg[c];
    return r;
  }

  Vector<Type, Dimension> operator-(Vector<Type, Dimension>& vArg)
  {
    Vector<Type, Dimension> r;
    for(uint c = 0; c < Dimension; c++)
      r[c] = v[c] - vArg[c];
    return r;
  }  
};

template<> class Vector<float, 2>
{
protected:
  float v[2];
public:  
  
  Vector() 
  {
    v[0] = 0.0f;
    v[1] = 0.0f;
  }
  Vector(float x, float y)
  {
    set(x,y);
  } 

  float& operator[](uint8 element) { return v[element]; }  

  float getLength() const
  {
    return (float)sqrt(v[0]*v[0] + v[1]*v[1]);
  }

  void set(float x, float y)
  {
    v[0] = x;
    v[1] = y;
  }

  void setLength(float length)
  { 
    if(!(getLength() > 0.0f))
      return;

    //Get multiplication factor
    float factor = length / getLength();
    
    //Apply factor to all elements    
    v[0] *= factor;
    v[1] *= factor;
  }
 
  /* clampLength
   *
   * Clamps the length of the vector to the value given by maxLength. Does nothing if the current
   * length is less than maxLength
   */
  void clampLength(float maxLength)
  {
    if(!(getLength() > 0.0f))
      return;

    //Get multiplication factor
    float factor = maxLength / getLength();
    
    if(factor < 1.0f)
    {
      //Apply factor to all elements    
      v[0] *= factor;
      v[1] *= factor;
    }
  }

  /* Casting operators */
  operator Vector<int,2> ()
  {
    Vector<int,2> r;
    for(uint c = 0; c < 2; c++)
      r[c] = (int)v[c];
    return r;
  }

  /* Mathematical operators */

  Vector& operator+=(Vector<float,2>& vector)
  {
    for(uint c = 0; c < 2; c++)
      v[c] += vector.v[c];
    return *this;
  }

  Vector operator+(Vector<float,2>& vector)
  {
    Vector<float,2> r;
    for(uint c = 0; c < 2; c++)
      r[c] = v[c] + vector.v[c];
    return r;
  }

  Vector operator*(float scalar)
  {
    Vector<float, 2> r;

    for(uint c = 0; c < 2; c++)
      r[c] = v[c] * scalar;
    return r;
  }
  Vector& operator/=(float scalar)
  {    
    for(uint c = 0; c < 2; c++)
      v[c] /= scalar;
    return *this;
  }
};



#endif

