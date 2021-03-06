﻿#pragma once
#include "BooleanNode.h"

class UnionNode: public BooleanNode
{
public:
	bool intersect(const Ray& r, ISect& i, std::stack<Geometry*>& intersections) const override;
	bool contains(bool intersections) const override;
};
