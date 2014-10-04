#include <iostream>
#include "includes/CImg.h"
#include "TileManager.h"
#include "TerrainMap.h"

using namespace cimg_library;
using namespace std;

void drawAt(CImg<unsigned char>& img, TileManager& manager, 
						TileManager::Tile t, int offsetX, int offsetY) {
	CImg<unsigned char>
  		&tImg = manager.getTile(t);

	img.draw_image(offsetX * TileManager::TileSize, 
								offsetY * TileManager::TileSize, 0, 0, 
								tImg, tImg.get_channel(3), 1, 255);

}

int main() {
  TileManager manager;
  TerrainMap tMap;

  tMap.loadMap("data/map.txt");

  CImg<unsigned char> image(tMap.getWidth() * TileManager::TileSize, 
  		tMap.getHeight() * TileManager::TileSize, 1, 3, 0);

  tMap.setTileManager(&manager);
  tMap.setDisplayView(&image);
  tMap.initView();

  
  drawAt(image, manager, TileManager::GREEN_TANK_L, 9, 4);
  drawAt(image, manager, TileManager::GREEN_TANK_U, 20, 22);
  drawAt(image, manager, TileManager::GREEN_TANK_U, 3, 25);
  drawAt(image, manager, TileManager::BLUE_TANK_L, 8, 4);
  drawAt(image, manager, TileManager::BLUE_TANK_L, 11, 8);
  drawAt(image, manager, TileManager::BLUE_TANK_L, 24, 22);

  CImgDisplay main_disp(image, "Run");

  bool tick = false;
  while (!main_disp.is_closed()) {

  	// if (tick) {
  	// 	//image.draw_image(48, 48, 0, 0,  t1);
  	// 	image.draw_image(9 * TileManager::TileSize, 4 * TileManager::TileSize, 0, 0, t2, t2.get_channel(3), 1, 255).display(main_disp);
  	// } else {
  	// 	//image.draw_image(48, 48, 0, 0,  t1);
  	// 	image.draw_image(8 * TileManager::TileSize, 4 * TileManager::TileSize, 0, 0, t3, t3.get_channel(3), 1, 255).display(main_disp);
  	// }

  	// tick = !tick;
  	// main_disp.wait(500);
  }
  return 0;
}