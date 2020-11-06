void kernel healthy_check(global const float3 *vectorArray,
                            const int X_MAX,
                            const int Y_MAX,
                            global int *numberCancerResult,
                            global float3 *vectorArrayResult){
    const int MUTATION_THRESHOLD = 6; 
    const int MUTATION_THRESHOLD_CORNER = 3;
    const int MUTATION_THRESHOLD_EDGE = 4;
    float3 RED = (float3)(1.0f,0.0f,0.0f);
    int numberCancer = 0;
    int i = get_global_id(0);
    int j = get_global_id(1);
    float3 color;
    if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
        if (i == 0 && j == 0) {
            color = vectorArray[2*((i*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;
            
            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;
        }else if (i == X_MAX && j == 0) { 

            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
               numberCancer++;
        }else if (j == Y_MAX && i == X_MAX) { 
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z) 
                numberCancer++;

        } else { 
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
                numberCancer++;
        } if (numberCancer >= MUTATION_THRESHOLD_CORNER){
            int index = 2*((i*Y_MAX)+j);
            color = vectorArray[index+1];
            if (!(color.x == RED.x)){
                vectorArrayResult[index] = vectorArray[index];
                vectorArrayResult[index + 1] = RED;
                numberCancerResult = atomic_inc(numberCancerResult);
            }
        }
    }else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
        /* only four options is left, bottom, right or top */
        if (j == 0) {
            /* top row */
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* left */
              numberCancer++;

             color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom-left */
              numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* right */
               numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom right */
               numberCancer++;
            
            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom */
               numberCancer++;

        } else if (i == X_MAX) { /* right side */

            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* up */
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* up-left */
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* left */
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom left */
               numberCancer++;

            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom */
               numberCancer++;

        } else if (j == Y_MAX) { /* bottom row*/
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* up */
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* up-left */
               numberCancer++;

            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* left */
               numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* up right */
               numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* right */
               numberCancer++;

        } else { /* left side */

            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom */
               numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* bottom-right */
               numberCancer++;
            
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* right */
               numberCancer++;

            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* up */
               numberCancer++;

            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
            if (color.x == RED.x && color.y == RED.y && color.z == RED.z)/* UP_RIGHT */
               numberCancer++;
        }

        if (numberCancer >= MUTATION_THRESHOLD_EDGE) {
            int index = 2*((i*Y_MAX)+j);
            color = vectorArray[index+1];
            if (!(color.x == RED.x)){
                vectorArrayResult[index] = vectorArray[index];
                vectorArrayResult[index + 1] = RED;
                numberCancerResult = atomic_inc(numberCancerResult);
            }
        }
    } else {
        /* check up */
        color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check top-left*/
        color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check top-right */
        color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check left */
        color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check right */
        color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check bottom-left */
        color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check bottom */
        color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;
        /* check bottom-right */
        color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
        if (color.x == RED.x && color.y == RED.y && color.z == RED.z)
           numberCancer++;


        if (numberCancer >= MUTATION_THRESHOLD) {
            int index = 2*((i*Y_MAX)+j);
            color = vectorArray[index+1];
            if (!(color.x == RED.x)){
                vectorArrayResult[index] = vectorArray[index];
                vectorArrayResult[index + 1] = RED;
                numberCancerResult = atomic_inc(numberCancerResult);
            }
        }
    }
}

void kernel cancer_check(global const float3 *vectorArray,
                        const int X_MAX,
                        const int Y_MAX,
                        global int *numberMedicalResult,
                        global int *numberCancerResult,
                        global float3 *vectorArrayResult){
   int numberMedical = 0;
   const int MUTATION_THRESHOLD = 6; 
   const int MUTATION_THRESHOLD_CORNER = 3;
   const int MUTATION_THRESHOLD_EDGE = 4;
   const float3 YELLOW = (float3)(1.0f,1.0f,0.0f);
   const float3 GREEN = (float3)(0.0f,0.0,0.4f);
   const float3 RED = (float3)(1.0f,0.0f,0.0f);
   int i = get_global_id(0);
   int j = get_global_id(1);
   float3 color;
   float3 medCellsBuffer[10];
   int bufferIndex = 0;
   /* Check for corners*/
   if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
      if (i == 0 && j == 0) { // top left corner
            // only three checks can be done
            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* down */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* down-right*/
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
        }else if (i == X_MAX && j == 0) { /* top right */
            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* down */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* left-down */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j+1))];
                bufferIndex++;
            }
        }else if (j == Y_MAX && i == X_MAX) { /* bottom right */
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j+1))];
                bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* left */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
                bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up-left */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j-1))];
                bufferIndex++;
            }
        }else { /* bottom left */
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){ /* up */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j-1))];
                bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* right */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j))];
                bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up-right */
                numberMedical++;
                medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j-1))];
                bufferIndex++;
            }
        }
        if (numberMedical >= MUTATION_THRESHOLD_CORNER) {
            color = vectorArray[2*(((i)*Y_MAX)+(j))+1];
            if(color.x == RED.x && color.y == RED.y){
               numberCancerResult = atomic_dec(numberCancerResult);
               vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = GREEN;
   
               for(int index = 0; index < bufferIndex; index++){
                  int x = medCellsBuffer[index].x;
                  int y = medCellsBuffer[index].y;
                  vectorArrayResult[2*(((x)*Y_MAX)+(y))+1] = GREEN;
               }
                  numberMedicalResult = atomic_sub(numberMedicalResult, numberMedical);
            }
        }else{
            vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = vectorArray[2*(((i)*Y_MAX)+(j))+1];
        }
   } 
   else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
        /* only four options is left, bottom, right or top */
        if (j == 0) {
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom-left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
        } else if (i == X_MAX) { /* right side */
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up-left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom lleft */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
        } else if (j == Y_MAX) { /* bottom row */
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up-left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* left */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* up right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
        } else { /* left side */
            color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
            if (color.y == YELLOW.y){/* bottom-right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j+1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
            if (color.y == YELLOW.y){/* right */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* U{P */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
            color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
            if (color.y == YELLOW.y){/* UP_RIGHT */
               numberMedical++;
               medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j-1))];
               bufferIndex++;
            }
        }

        if (numberMedical >= MUTATION_THRESHOLD_EDGE) {
           color = vectorArray[2*(((i)*Y_MAX)+(j))+1];
            if(color.x == RED.x && color.y == RED.y){
               numberCancerResult = atomic_dec(numberCancerResult);
               vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = GREEN;

               for(int index = 0; index < bufferIndex; index++){
                  int x = medCellsBuffer[index].x;
                  int y = medCellsBuffer[index].y;
                  vectorArrayResult[2*(((x)*Y_MAX)+(y))+1] = GREEN;
               }
                  numberMedicalResult = atomic_sub(numberMedicalResult, numberMedical);
            }
        }else{
            vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = vectorArray[2*(((i)*Y_MAX)+(j))+1];
        }
   }
    else {
        color = vectorArray[2*(((i)*Y_MAX)+(j-1))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j-1))];
            bufferIndex++;
        }
        /* check top-left */
        color = vectorArray[2*(((i-1)*Y_MAX)+(j-1))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j-1))];
            bufferIndex++;
        }
        /* check top-right */
         color = vectorArray[2*(((i+1)*Y_MAX)+(j-1))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j-1))];
            bufferIndex++;
        }
        /* check left */
        color = vectorArray[2*(((i-1)*Y_MAX)+(j))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j))];
            bufferIndex++;
        }
        /* check right */
        color = vectorArray[2*(((i+1)*Y_MAX)+(j))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j))];
            bufferIndex++;
        }
        /* check bottom-left */
        color = vectorArray[2*(((i-1)*Y_MAX)+(j+1))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i-1)*Y_MAX)+(j+1))];
            bufferIndex++;
        }
        /* check bottom */
        color = vectorArray[2*(((i)*Y_MAX)+(j+1))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i)*Y_MAX)+(j+1))];
            bufferIndex++;
        }
        /* check bottom-right */
        color = vectorArray[2*(((i+1)*Y_MAX)+(j+1))+1];
        if (color.y == YELLOW.y){
            numberMedical++;
            medCellsBuffer[bufferIndex] = vectorArray[2*(((i+1)*Y_MAX)+(j+1))];
            bufferIndex++;
        }

        if (numberMedical >= MUTATION_THRESHOLD) {
           color = vectorArray[2*(((i)*Y_MAX)+(j))+1];
            if(color.x == RED.x && color.y == RED.y){
               numberCancerResult = atomic_dec(numberCancerResult);
               vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = GREEN;

               for(int index = 0; index < bufferIndex; index++){
                  int x = medCellsBuffer[index].x;
                  int y = medCellsBuffer[index].y;
                  vectorArrayResult[2*(((x)*Y_MAX)+(y))+1] = GREEN;
               }
                  numberMedicalResult = atomic_sub(numberMedicalResult, numberMedical);
            }
        }else{
            vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = vectorArray[2*(((i)*Y_MAX)+(j))+1];
      }
   }  
}