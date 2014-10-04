#include <iostream>
#include "include/CImg.h"
#include "TileManager.h"
#include "TerrainMap.h"
#include "AppConfig.h"

#include <stdlib.h>
#include <time.h>
#include <set>

using namespace cimg_library;
using namespace std;

// void drawAt(CImg<unsigned char>& img, TileManager& manager, 
// 						string t, int offsetX, int offsetY) {
// 	CImg<unsigned char>
//   		&tImg = manager.getTile(t);

// 	img.draw_image(offsetX * TileManager::TileSize, 
// 								offsetY * TileManager::TileSize, 0, 0, 
// 								tImg, tImg.get_channel(3), 1, 255);

// }
struct intPair {
  bool operator()(pair<int,int> p1, pair<int,int> p2) {
    int s1 = p1.first,
        e1 = p1.second,
        s2 = p2.first,
        e2 = p2.second;
         
    return (s1 < s2) || ((s1 == s2) && (e1 < e2));
  }
};



int main() {

  srand(time(NULL));

  AppConfig::loadConfig("app.config");

  TileManager manager(AppConfig::getConfig("tile"));
  TerrainMap tMap;

  tMap.loadMap(AppConfig::getConfig("map").c_str());

  int tilesize = manager.getTileSize();

  CImg<unsigned char> image(tMap.getWidth() * tilesize, 
  		tMap.getHeight() * tilesize, 1, 3, 0);

  tMap.setTileManager(&manager);
  tMap.setDisplayView(&image);
  tMap.initView();


  CImgDisplay main_disp(image, "Run");

  bool tick = false;
  int numOfFire = 15;

  set<pair<int, int>, intPair> lastFires;
  set<pair<int, int>, intPair>::const_iterator it;
  int w = tMap.getWidth(), h = tMap.getHeight();

  vector<string> fireTypes;
  fireTypes.push_back("EFFECT.FIRE_S");
  fireTypes.push_back("EFFECT.FIRE_M");
  fireTypes.push_back("EFFECT.BULLET_S");
  fireTypes.push_back("EFFECT.BULLET_M");
  int fireTypesNo = fireTypes.size();

  while (!main_disp.is_closed()) {
    main_disp.wait(300);


    // undo last fires
    for (it = lastFires.begin(); it != lastFires.end(); ++it) {
      tMap.removeFire((*it).first, (*it).second);
      cout << "removeFire fire: " << (*it).first << " " << (*it).second << endl;
    }

    lastFires.clear();

    // new fires
    for (int i = 0; i < numOfFire; i++) {
      int offsetX, offsetY;

      do {
        offsetX = rand() % w;
        offsetY = rand() % h;  
      } while (lastFires.find(pair<int,int>(offsetX, offsetY)) != lastFires.end());
      cout << "insert fire: " << offsetX << " " << offsetY << endl;
      lastFires.insert(pair<int,int>(offsetX, offsetY));
      tMap.addFire(offsetX, offsetY, fireTypes[rand()%fireTypesNo]);
    }

    image.display(main_disp);
  }
  return 0;
}