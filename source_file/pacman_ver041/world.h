#ifndef WORLD_H
#define WORLD_H

#include "global.h"

struct Star{
	Location mLocation;
	glm::mat4 mTranslationMat;
	bool isShow;
};

class World{
private:
	std::vector<Model> mapModel_;
	std::vector<Model> myCharacter_;
	std::vector<Model> starModel_;
	std::vector<Star> stars_;
	std::vector<Star> starsInitialCopy_;	// holds a copy of initial status for stars, easy to reset the World
	bool **map_;
	CharactorStatus myCurrentStatus_;
	Direction myCurrentDirection_;
	Location myLocation_;
	Location myLocationInitialCopy_;

	Location GetNextMove();
	void ReadMapFile(const char *mapFileName);

public:
	World(Location chaLocation, const char *mapFile);
	~World();
	
	void Reset();

	void SetMapModel(std::vector<Model> &model);
	std::vector<Model> GetMapModel();

	void SetMyCharacterModel(std::vector<Model> &model);
	std::vector<Model> GetMyCharacterModel();

	void SetStarModel(std::vector<Model> &model);
	std::vector<Model> GetStarModel();

	void AddStar(Location maploc, glm::vec3 translate_xyz);
	void UpdateStar(size_t starIdx);
	std::vector<Star> GetStars();

	Location GetMyLocation();
	Location NextPosition();
	void UpdateMyLocation();

	void UpdateCurrentDirection(Direction dir);
	Direction GetCurrentDirection();

	bool IsNextPositionAccessible();

	void SetCurrentStatus(CharactorStatus status);
	CharactorStatus GetCurrentStatus();
};

#endif	// end WORLD_H
