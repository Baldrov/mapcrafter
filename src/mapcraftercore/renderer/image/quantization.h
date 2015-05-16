/*
 * Copyright 2012-2015 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGE_QUANTIZATION_H_
#define IMAGE_QUANTIZATION_H_

#include "palette.h"
#include "../image.h"
#include "../../util.h"

#include <vector>

namespace mapcrafter {
namespace renderer {

class Octree;

class Octree {
public:
	Octree(Octree* parent = nullptr);
	~Octree();

	Octree* getParent();
	const Octree* getParent() const;

	bool isLeaf() const;

	bool hasChildren(int index) const;
	int getChildrenCount() const;
	Octree* getChildren(int index);
	const Octree* getChildren(int index) const;

	bool hasColor() const;
	RGBAPixel getColor() const;
	void setColor(RGBAPixel color);
	void reduceColor();

	int getColorID() const;
	void setColorID(int color_id);

	void updateParents();

	static Octree* findOrCreateNode(Octree* octree, RGBAPixel color);

	/**
	 * Traverses the octree to the node of a given color and stops and returns it when a node
	 * with a color is found.
	 *
	 * TODO Warning, won't work if path to color doesn't contain a node with color
	 */
	static const Octree* findNearestNode(const Octree* octree, RGBAPixel color);
	
	static int findNearestColor(const Octree* octree, RGBAPixel color);

protected:
	Octree* parent;
	Octree* children[8];

	int reference;
	int red, green, blue;
	int color_id;

	std::vector<std::pair<int, RGBAPixel>> subtree_colors;
};

class OctreePalette : public Palette {
public:
	OctreePalette(const std::vector<RGBAPixel>& colors);
	virtual ~OctreePalette();

	virtual const std::vector<RGBAPixel>& getColors() const;
	virtual int getNearestColor(const RGBAPixel& color) const;

protected:
	std::vector<RGBAPixel> colors;

	Octree octree;
};

/**
 * Quantizes the colors of a given image.
 */
void octreeColorQuantize(const RGBAImage& image, size_t max_colors, std::vector<RGBAPixel>& colors, Octree** octree = nullptr);

}
}

#endif /* IMAGE_QUANTIZATION_H_ */
