CC 		= g++
TARGET 		= Styled
OBJ 		= disjoint-set.o Segmentation.o RegionBasedColorTransfer.o main.o 
CFLAGS          =  -Wall -g -O2 -std=c++11
DIR 		= -I/usr/local/Cellar/opencv/2.4.12_2/include/opencv -I/usr/local/Cellar/opencv/2.4.12_2/include
LIBS 		= -L/usr/local/Cellar/opencv/2.4.12_2/lib-L/usr/local/Cellar/opencv/2.4.12_2/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab


$(TARGET):$(OBJ)
	$(CC) $(CFLAGS) $(DIR) $(LIBS) -o $@ $(OBJ)
%.o : %.cpp
	$(CC) $(CFLAGS) $(DIR) $(LIBS) -o $@ -c $<

.PHONY:clean

clean:
	rm -fr *.o $(TARGET)
