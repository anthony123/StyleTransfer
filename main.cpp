#include "cv.h"
#include "RegionBasedColorTransfer.h"
int main()
{
  char sourceName[] = "source.jpeg";
  char targetName[] = "target.jpeg";
  RegionBasedColorTransfer rbct(sourceName, targetName);
  rbct.process();
  rbct.showTarget();
  //rbct.showSource();

  return 0;
}
