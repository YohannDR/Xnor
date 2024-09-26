%module CoreNative

%import "typemaps/cs_struct.i"

%{
#include "Maths/math.hpp"
%}

%include "Maths/definitions.hpp"

%include "Maths/calc.hpp"

%include "Maths/easing.hpp"

%ignore Vector2;
%ignore Vector2i;
%ignore Vector3;
%ignore Vector4;
%ignore Quaternion;
%ignore Matrix;
%ignore Matrix3;

%cs_struct2(Vector2, Data8, Vector2);
%cs_struct2(Vector2i, Data8, Vector2i);
%cs_struct2(Vector3, Data12, Vector3);
%cs_struct2(Vector4, Data16, Vector4);
%cs_struct2(Quaternion, Data16, Quaternion);
%cs_struct2(Matrix3, Data36, Matrix3);
%cs_struct2(Matrix, Data64, Matrix);

class Vector2 {};
class Vector2i {};
class Vector3 {};
class Vector4 {};
class Quaternion {};
class Matrix {};
class Matrix3 {};
