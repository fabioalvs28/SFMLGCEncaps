#pragma once
#include "Vectors.h"

struct GCMatrix
{

	GCMatrix(); //constructeur par defaut

	GCMatrix(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44);

	~GCMatrix() = default;

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	static GCMatrix Identity() { return GCMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); };
	static GCMatrix Zero() { return GCMatrix(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); };
	void operator*=(const GCMatrix& other); //multiplie la matrice par une autre matrice

	void SetZero(); //set les valeurs de la matrice a 0
	void SetIdentity(); //set les valeurs de la matrice a l'identite
	void Pow(int exp);
	float Determinant(); //calcule le determinant de la matrice
	void Inverse(); //inverse la matrice
	void Transpose(); //transpose la matrice

	void Translation(float x, float y, float z); //translation la matrice
	void Rotation(GCVEC3 position, float angle); //rotation la matrice
	void Scale(float x, float y, float z); //scale la matrice

};