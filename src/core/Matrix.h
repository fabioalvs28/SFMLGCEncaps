#pragma once

struct GCVEC3;

struct GCMATRIX
{

	GCMATRIX(); //constructeur par defaut

	GCMATRIX(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44);

	~GCMATRIX() = default;

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	static GCMATRIX Identity() { return GCMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); };
	static GCMATRIX Zero() { return GCMATRIX(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); };
	void operator*=(const GCMATRIX& other); //multiplie la matrice par une autre matrice

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