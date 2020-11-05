void kernel healthy_check(global const float3 *vectorArray,
                            const int X_MAX,
                            const int Y_MAX,
                            global int numberCancerResult,
                            global float3 *vectorArrayResult){

    const int MUTATION_THRESHOLD = 6; 
    const int MUTATION_THRESHOLD_CORNER = 3;
    const int MUTATION_THRESHOLD_EDGE = 4;
    float3 RED = (float3)(1.0f,0.0f,0.0f);
    int numberCancer = 0;
    int i = get_global_id(0);
    int j = get_global_id(1);
    if ((i == 0 || i == X_MAX) && (j == 0 || j == Y_MAX)) {
        float3 color;
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
        } else if (j == Y_MAX && i == X_MAX) { 
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
        }
        if (numberCancer >= MUTATION_THRESHOLD_CORNER) {
            int index = 2*((i+*Y_MAX)+j);
            vectorArrayResult[index] = vectorArray[index];
            vectorArrayResult[index + 1] = RED;
            numberCancerResult = atomic_inc(numberCancerResult);
        }
    }
}







/////////////////////
else if (i == 0 || i == X_MAX || j == 0 || j == Y_MAX) {
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
            vectorArrayResult[index] = vectorArray[index];
            vectorArrayResult[index + 1] = RED;
            numberCancerResult = atomic_inc(numberCancerResult);
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
            vectorArrayResult[index] = vectorArray[index];
            vectorArrayResult[index + 1] = RED;
            numberCancerResult = atomic_inc(numberCancerResult);
        }
    }

    /** else{
            vectorArrayResult[2*(((i)*Y_MAX)+(j))+1] = vectorArray[2*(((i)*Y_MAX)+(j))+1];
        } */