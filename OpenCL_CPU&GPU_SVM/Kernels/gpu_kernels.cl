#define MUTATION_THRESHOLD  6
#define MUTATION_THRESHOLD_CORNER   3
#define MUTATION_THRESHOLD_EDGE     4
#define RED (float3)(1.0f,0.0f,0.0f)
#define YELLOW (float3)(1.0f,1.0f,0.0f)
#define GREEN (float3)(0.0f,0.8,0.4f)
#define UP  0
#define UP_RIGHT    1
#define UP_LEFT 2
#define DOWN    3
#define DOWN_RIGHT  4
#define DOWN_LEFT   5
#define LEFT    6
#define RIGHT   7

void kernel mutation_kernel(global float3 *vectorArray,
                            const int X_MAX,
                            const int Y_MAX,
                            global int *numberCancerResult,
                            global int *numberMedicalResult){

    const int i = get_global_id(0);
    const int j = get_global_id(1);                          
    int numberCancer = 0;
    bool top_edge = j == 0;
    bool left_edge = i == 0;
    bool right_edge = i == X_MAX;
    bool bottom_edge = j == Y_MAX;
    bool isCorner = (top_edge && left_edge) || (top_edge && right_edge) || (bottom_edge && left_edge) || (bottom_edge && right_edge);
    bool isEdge = top_edge || left_edge || right_edge || bottom_edge;
    float3 color = vectorArray[2*((i*Y_MAX)+(j))+1]; 
    if(color.x == GREEN.x && color.y == GREEN.y && color.z == GREEN.z){
        // check top cell
        if(!top_edge){
            color = vectorArray[2*((i*Y_MAX)+(j-1))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check top left
        if(!top_edge && !left_edge){
            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check top right
        if(!top_edge && !right_edge){
            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check bottom cell
        if(!bottom_edge){
            color = vectorArray[2*((i*Y_MAX)+(j+1))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check left cell
        if(!left_edge){
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check right cell
        if(!right_edge){
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check bottom left
        if(!bottom_edge && !left_edge){
            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }
        // check bottom right
        if(!bottom_edge && !right_edge){
            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1]; 
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                    numberCancer++;
        }

        if ((isCorner && numberCancer >= MUTATION_THRESHOLD_CORNER)||
        (isEdge && numberCancer >= MUTATION_THRESHOLD_EDGE)||
        (numberCancer >= MUTATION_THRESHOLD)){
                int index = 2*((i*Y_MAX)+j);
                color = vectorArray[index+1];
                if (!(color.x == RED.x)){
                    vectorArray[index + 1] = RED;
                    atomic_inc(numberCancerResult);
                }
        }
    }else if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
        int numberMedical = 0;
        float3 medCellsBuffer[10];

        // check top cell
        if(!top_edge){
            color = vectorArray[2*((i*Y_MAX)+(j-1))+1]; 
            if (color.y == YELLOW.y){
                medCellsBuffer[numberMedical] = vectorArray[2*(((i)*Y_MAX)+(j-1))];
                numberMedical++; 
            }
        }
        // check top left
        if(!top_edge && !left_edge){
            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1]; 
            if (color.y == YELLOW.y){
                medCellsBuffer[numberMedical] = vectorArray[2*(((i-1)*Y_MAX)+(j-1))];
                numberMedical++;
            } 
        }
        // check top right
        if(!top_edge && !right_edge){
            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1]; 
            if (color.y == YELLOW.y) {
                medCellsBuffer[numberMedical] = vectorArray[2*(((i+1)*Y_MAX)+(j-1))];
                numberMedical++;
            } 
        }
        // check bottom cell
        if(!bottom_edge){
            color = vectorArray[2*((i*Y_MAX)+(j+1))+1]; 
            if (color.y == YELLOW.y){
                medCellsBuffer[numberMedical] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
                numberMedical++;
            }
        }
        // check left cell
        if(!left_edge){
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1]; 
            if (color.y == YELLOW.y){
                medCellsBuffer[numberMedical] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
                numberMedical++;
            }
        }
        // check right cell
        if(!right_edge){
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1]; 
            if (color.y == YELLOW.y) {
                medCellsBuffer[numberMedical] = vectorArray[2*(((i+1)*Y_MAX)+(j))];
                numberMedical++;
            }
                  
        }
        // check bottom left
        if(!bottom_edge && !left_edge){
            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1]; 
            if (color.y == YELLOW.y){
                medCellsBuffer[numberMedical] = vectorArray[2*(((i-1)*Y_MAX)+(j+1))];
                numberMedical++;
            }
        }
        // check bottom right
        if(!bottom_edge && !right_edge){
            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1]; 
            if (color.y == YELLOW.y){
                medCellsBuffer[numberMedical] = vectorArray[2*(((i+1)*Y_MAX)+(j+1))];
                numberMedical++;
            }       
        }
        
         if ((isCorner && numberMedical >= MUTATION_THRESHOLD_CORNER)||
        (isEdge && numberMedical >= MUTATION_THRESHOLD_EDGE)||
        (numberMedical >= MUTATION_THRESHOLD)){
            color = vectorArray[2*(((i)*Y_MAX)+(j))+1];
            if(color.x == RED.x && color.y == RED.y){
               atomic_dec(numberCancerResult);
               vectorArray[2*(((i)*Y_MAX)+(j))+1] = GREEN;
   
               for(int index = 0; index < numberMedical; index++){
                  int x = medCellsBuffer[index].x;
                  int y = medCellsBuffer[index].y;
                  vectorArray[2*(((x)*Y_MAX)+(y))+1] = GREEN;
               }
                //atomic_sub(numberMedicalResult, numberMedical);
            }
        }
    }
}


void kernel radial_expansion(global float3 *vectorArray,
                            global int* direction, 
                            const int X_MAX,
                            const int Y_MAX,
                            global int *numberMedicalResult,
                            global int *numberCancerResult){


   const int x = get_global_id(0);
   const int y = get_global_id(1);
   float3 color;

    int mindex = 2*((x*Y_MAX)+(y));
    color = vectorArray[mindex+1];
    int dirIndex =  (x*Y_MAX) + y;
    int dir = direction[dirIndex];
    
    if(color.x == YELLOW.x && color.y == YELLOW.y && color.z == YELLOW.z){
      if(dir == UP){
         if(y > 0){
               int index = 2*((x*Y_MAX)+(y-1));
               color = vectorArray[index+1];
               if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
                  atomic_dec(numberCancerResult);
               }
               
               vectorArray[index+1] = YELLOW;
               direction[(x*Y_MAX)+(y-1)] = UP;
         }else {
               atomic_dec(numberMedicalResult);
         }
         vectorArray[mindex+1] = GREEN;
      }
      else if(dir == UP_RIGHT){
        if(x < X_MAX && y > 0){
            int index = 2*(((x+1)*Y_MAX)+(y-1));
            
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
                atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x+1)*Y_MAX)+(y-1)] = UP_RIGHT;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
    else if(dir == UP_LEFT){
        if(x > 0 && y > 0){
            int index = 2*(((x-1)*Y_MAX)+(y-1));
            
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
               atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x-1)*Y_MAX)+(y-1)] = UP_LEFT;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
    else if(dir == RIGHT){
        if(x < X_MAX){
            int index = 2*(((x+1)*Y_MAX)+(y));
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
               atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x+1)*Y_MAX)+(y)] = RIGHT;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
    else if(dir == LEFT){
        if(x > 0){
            int index = 2*(((x-1)*Y_MAX)+(y));
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
               atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x-1)*Y_MAX)+(y)] = LEFT;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
    else if(dir == DOWN){
        if(y < Y_MAX){
            int index = 2*(((x)*Y_MAX)+(y+1));
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
               atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x)*Y_MAX)+(y+1)] = DOWN;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
    else if(dir == DOWN_RIGHT){
        if(x < X_MAX && y < Y_MAX){
            int index = 2*(((x+1)*Y_MAX)+(y+1));
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
               atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x+1)*Y_MAX)+(y+1)] = DOWN_RIGHT;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
    else if(dir == DOWN_LEFT){
        if(x >0 && y < Y_MAX){
            int index = 2*(((x-1)*Y_MAX)+(y+1));
            color = vectorArray[index+1];
            if(color.x == RED.x && color.y == RED.y && color.z == RED.z){
                atomic_dec(numberCancerResult);
            }
            vectorArray[index+1] = YELLOW;
            direction[((x-1)*Y_MAX)+(y+1)] = DOWN_LEFT;
        }else {
            atomic_dec(numberMedicalResult);
        }
        vectorArray[mindex+1] = GREEN;
    }
   }
}