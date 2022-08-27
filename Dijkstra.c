#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define MAX_VERTEX 5
typedef struct{
    int distance;
    int previous;
    int visited;
    char label[50];
} Vertex;

void InsertVertex(char vertexLabel[5], Vertex VertexArray[MAX_VERTEX], int *totalVertex);
void FileHandler(int *totalVertex, int argc, char *argv[], int Adjacency[MAX_VERTEX][MAX_VERTEX], Vertex VertexArray[MAX_VERTEX]);
void Dijkstra(Vertex VertexArray[MAX_VERTEX], int AdjMatrix[][MAX_VERTEX],int VertexCount, int StartVertex);
int main(int argc, char *argv[]){
    int totalVertex =0;
    char startVertexAsCHAR;
    int startVertexAsINT;

    char endVertexAsCHAR;
    int endVertexAsINT;

    //Declaring and Initializing the Adjacency Matrix
    int AdjacencyMatrix[MAX_VERTEX][MAX_VERTEX];
    for (int i=0;i<MAX_VERTEX;i++){
        for (int j=0;j<MAX_VERTEX;j++){
            AdjacencyMatrix[i][j] = -1;
        }
    }
    //Declaring a "Vertex" Array
    Vertex VertexArray[MAX_VERTEX];

    FileHandler(&totalVertex,argc, argv, AdjacencyMatrix, VertexArray);
    printf("After reading the file our total vertex is %d", totalVertex);


    //Setting the initial distance for all the vertex as INFINITY
    for (int i=0; i<totalVertex;i++){
        VertexArray[i].distance = INT_MAX;
    }

    //CHOOSING THE STARTING VERTEX
    printf("What is the starting Vertex?");
    scanf("%s ", startVertexAsCHAR);
    for (int i=0; i<totalVertex;i++){
        if (strcmp(VertexArray[i].label, startVertexAsCHAR)==0){
            startVertexAsINT = i;
        }
    }

    //CHOOSING THE ENDING VERTEX
    printf("What is the Ending Vertex?");
    scanf("%s ", endVertexAsCHAR);
    for (int i=0; i<totalVertex;i++){
        if (strcmp(VertexArray[i].label, endVertexAsCHAR)==0){
            endVertexAsINT= i;
        }
    }

    //if the vertex is valid then
    if (startVertexAsINT != -1 || endVertexAsINT!=-1){
        //this will populate our VertexArray
        Dijkstra(VertexArray, AdjacencyMatrix, totalVertex, startVertexAsINT);

        //we need to print the path now
        int path[MAX_VERTEX];
        //putting end of destination vertex as path[0]
        path[0] = endVertexAsINT;
        int pathTracker=0;
        //until we get to the
        while (path[pathTracker] != -1){
            path[pathTracker]= VertexArray[path[pathTracker-1]].previous;
        }

        printf("The path from %s to %s is %s",
               VertexArray[startVertexAsINT].label, VertexArray[endVertexAsINT].label, VertexArray[path[pathTracker-1]].label);

        for(int j = pathTracker-2 ; j>=0; j--)
        {
            printf("->%s", VertexArray[path[j]].label);
        }
        printf(" and has a length of %d.", VertexArray[endVertexAsINT].distance);


    }

    return 0;
}

void FileHandler(int *totalVertex, int argc, char *argv[], int AdjacencyMatrix[MAX_VERTEX][MAX_VERTEX], Vertex VertexArray[MAX_VERTEX]){
    FILE *myFile = NULL;
    char file_name[100];
    char file_line[10];
    char *token;

    char label[5];
    int start;
    int end;
    int weight;

    argc = 2;
    argv[1] = "TestFile.txt";
    //checking for number of command line arguments
    if (argc>=2){

        //file name was provided -> copy it into file variable
        //strcpy(file_name, argv[1]);
        strcpy(file_name, argv[1]);
        myFile = fopen(file_name, "r");
        //we check if the file pointer is valid

         if (myFile != NULL){

            //this will read the example line A,1,3,3,6 (Label, end, weight, end, weight)

            //making sure the end of line is a null character so that the line is tokenized as a string


            //parsing the line
            while (fgets(file_line, sizeof(file_line) - 1, myFile))
            {

                if (file_line[strlen(file_line)-1] == '\n')
                {
                    file_line[strlen(file_line)-1] = '\0';
                }
                //ensure that we read the entire line no matter the length
                token = strtok(file_line, ",");
                strcpy(label, token); //A - label
                while(token) {
                    start = (*totalVertex); //the first line will be the start
                    token = strtok(NULL, ",");
                    end = atoi(token); //1 - End
                    token = strtok(NULL, ",");
                    weight = atoi(token); //3 - Weight

                    AdjacencyMatrix[start][end] = weight;
                    #ifdef UNDIRECTED
                    AdjMatrix[end][start] = weight;
                    #endif

                    InsertVertex(label, VertexArray, totalVertex); //A - Label
                    (*totalVertex)++;
                }

            }


            fclose(myFile);
        }else{
             printf("Invalid file...exiting\n");
             exit(0);
        }

    } else{
        //file name wasn't provided
        printf("File Name needed!\n");
        exit(0);
    }

}
void Dijkstra(Vertex VertexArray[MAX_VERTEX], int AdjMatrix[][MAX_VERTEX],int VertexCount, int StartVertex)
{
    int CurrentVertex = StartVertex;
    VertexArray[StartVertex].distance = 0;
    VertexArray[StartVertex].previous = -1;
    VertexArray[StartVertex].visited = 1;
    int dofu,cofuv,dofv;
    int x,i;

    for (x = 0; x < VertexCount-1; x++)
    {
        for(i = 0; i < VertexCount; i++)
        {

            if (AdjMatrix[CurrentVertex][i] != -1 && !VertexArray[i].visited)
            {
                dofu = VertexArray[CurrentVertex].distance;
                cofuv = AdjMatrix[CurrentVertex][i];
                dofv = VertexArray[i].distance;
                if (dofu + cofuv < dofv)
                {
                    dofv = dofu + cofuv;
                    VertexArray[i].distance = dofv;
                    VertexArray[i].previous = CurrentVertex;
                }
            }
        }



        for(i=0; i<VertexCount; i++)
        {
            int SmallestVertexIndex = -1;
            int SmallestVertex = INT_MAX;
            for(i = 0; i < VertexCount; i++)
            {
                if (!VertexArray[i].visited)
                {
                    if (VertexArray[i].distance < SmallestVertex)
                    {
                        SmallestVertex = VertexArray[i].distance;
                        SmallestVertexIndex = i;
                    }
                }
            }
            CurrentVertex = SmallestVertexIndex;
            VertexArray[CurrentVertex].visited = 1;
        }

    }
}

void InsertVertex(char vertexLabel[5], Vertex VertexArray[MAX_VERTEX], int *totalVertex){
    Vertex newVertex; //created a new vertex
    strcpy(newVertex.label, vertexLabel); //set the label
    newVertex.visited = 0;
    VertexArray[*totalVertex] = newVertex;
}

