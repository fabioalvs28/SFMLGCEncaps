#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START(Test)

void Start() override { count = 0; }
void Update() override;
public:
	GCText* pText;
	int count;

CREATE_SCRIPT_END