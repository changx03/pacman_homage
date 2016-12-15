#include "world.h"
#include "math_matrix.h"

World::World(Location chaLocation, const char *mapFile){
	myCurrentStatus_ = IDLE;
	myCurrentDirection_ = DOWN;
	myLocation_ = myLocationInitialCopy_ = chaLocation;
	
	ReadMapFile(mapFile);
}

World::~World(){
	free(map_);

	// clear vao, vbo for each model
	for (size_t i = 0; i < mapModel_.size(); i++){
		mapModel_[i].CleanUp();
	}
	for (size_t i = 0; i < myCharacter_.size(); i++){
		myCharacter_[i].CleanUp();
	}
	for (size_t i = 0; i < starModel_.size(); i++){
		starModel_[i].CleanUp();
	}
}

void World::Reset(){
	myLocation_ = myLocationInitialCopy_;
	myCurrentStatus_ = IDLE;
	myCurrentDirection_ = DOWN;
	stars_.clear();
	stars_.assign(starsInitialCopy_.begin(), starsInitialCopy_.end());
}

void World::SetMapModel(std::vector<Model> &model) {
	mapModel_.assign(model.begin(), model.end());
	model.clear();
}

std::vector<Model> World::GetMapModel(){ return mapModel_; }

void World::SetMyCharacterModel(std::vector<Model> &model) {
	myCharacter_.assign(model.begin(), model.end());
	model.clear();
}

std::vector<Model> World::GetMyCharacterModel(){ return myCharacter_; }

void World::SetStarModel(std::vector<Model> &model){
	starModel_.assign(model.begin(), model.end());
	model.clear();
}

std::vector<Model> World::GetStarModel(){ return starModel_; }

void World::AddStar(Location maploc, glm::vec3 translate_xyz){
	Star myStar;
	myStar.mLocation = maploc;
	myStar.mTranslationMat = math_matrix::GetModelTranslation(translate_xyz);
	myStar.isShow = true;

	stars_.push_back(myStar);
	starsInitialCopy_.push_back(myStar);
}

std::vector<Star> World::GetStars(){ return stars_; }

void World::UpdateStar(size_t starIdx){
	if (starIdx < stars_.size()){
		stars_[starIdx].isShow = false;
	}
}

Location World::GetMyLocation(){
	return myLocation_;
}

Location World::NextPosition(){
	Location nextMove = GetNextMove();
	Location nextPosition;

	nextPosition.x = myLocation_.x + nextMove.x;
	nextPosition.y = myLocation_.y + nextMove.y;
	return nextPosition;
}

Location World::GetNextMove(){
	Location nextMove;

	switch (myCurrentDirection_)
	{
	case DOWN:
		nextMove = CreateLocation(0, 1);
		break;
	case LEFT:
		nextMove = CreateLocation(1, 0);
		break;
	case UP:
		nextMove = CreateLocation(0, -1);
		break;
	case RIGHT:
		nextMove = CreateLocation(-1, 0);
		break;
	default:
		nextMove = CreateLocation(0, 0);
		break;
	}
	return nextMove;
}

void World::UpdateCurrentDirection(Direction dir){
	int temp;
	switch (dir){
	case LEFT:
		temp = (int)myCurrentDirection_ + 1;
		temp = temp % 4;
		myCurrentDirection_ = (Direction)temp;
		break;
	case RIGHT:
		temp = (int)myCurrentDirection_ - 1;
		if (temp == -1) temp = 3;
		temp = temp % 4;
		myCurrentDirection_ = (Direction)temp;
		break;
	case DOWN:
		temp = (int)myCurrentDirection_ + 2;
		temp = temp % 4;
		myCurrentDirection_ = (Direction)temp;
		break;
	default:
		break;
	}
	std::cout << "dir: " << Dir2String(myCurrentDirection_) << std::endl;
}

Direction World::GetCurrentDirection(){ 
	return myCurrentDirection_; 
}

bool World::IsNextPositionAccessible(){
	Location nextPosition = NextPosition();
	if (map_[nextPosition.y][nextPosition.x])
		return true;
	else
		return false;
}

void World::UpdateMyLocation(){
	myLocation_ = NextPosition();
}

void World::SetCurrentStatus(CharactorStatus status){
	myCurrentStatus_ = status;
}

CharactorStatus  World::GetCurrentStatus(){ 
	return myCurrentStatus_; 
}

void World::ReadMapFile(const char *mapFileName){

	std::ifstream mapFile(mapFileName, std::ios::binary);
	if (!mapFile.is_open()){
		std::cerr << "Error: Unable to open shader file" << std::endl;
		return;
	}

	// read map size
	size_t rows;
	size_t cols;
	std::string currentStr;
	std::getline(mapFile, currentStr);
	std::stringstream mStream;
	mStream << currentStr;
	mStream >> rows;	// y
	mStream >> cols;	// x
	std::cout << "rows = " << rows << " cols = " << cols << std::endl;
	//getchar();

	map_ = (bool**)malloc(rows * sizeof(bool*));
	for (size_t i = 0; i < rows; i++){
		map_[i] = (bool*)malloc(cols * sizeof(bool));
	}

	// read map value
	for (size_t y = 0; y < rows; y++){
		std::getline(mapFile, currentStr);
		mStream << currentStr;
		for (size_t x = 0; x < cols; x++){
			mStream >> map_[y][x];
		}
	}

	for (size_t y = 0; y < rows; y++){
		for (size_t x = 0; x < cols; x++){
			std::cout << map_[y][x];
		}
		std::cout << std::endl;
	}
	//getchar();
	mapFile.close();
}