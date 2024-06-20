
//Copyright (c) Daniel Dickson Dillimono
//Study on unit vectors assignment.

/*
    A program that populates an area in 2D with objects
    and find this optimal orientation to be in inorder to see
    as many objects as possible from a given position.
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

#define PI 3.1415
struct Person{
    double xCoordinate;
    double yCoordinate;
    double fieldOfViewAngle; //Note: obviously should not be greater than 360
};

struct Tree{
    double xCoordinate;
    double yCoordinate;
};

struct OrientationVector{
    double xCoordinate;
    double yCoordinate;
};

/*!
    Returns a unit vector when x and y coordinates are known
*/
OrientationVector getOrientationUnitVector(const double &xCoordinate, const double &yCoordinate)
{
    OrientationVector orientationVector;
    double hypotenuse = 0;
    hypotenuse = std::sqrt(xCoordinate*xCoordinate + yCoordinate*yCoordinate);
    orientationVector = OrientationVector{xCoordinate/hypotenuse , yCoordinate/hypotenuse};
    return orientationVector;
}

/*!
    Returns a unit vector when angle is known
*/
OrientationVector getOrientationUnitVectorCoordinates(int angle)
{
    OrientationVector orientationVector;
    int vectorAngle = 0;
    if(angle < 90){ //First quadrant
        vectorAngle = angle;
    }
    else if(angle > 90 && angle < 180){ //Second quadrant
        vectorAngle = 180 - angle;
    }
    else if(angle > 180 && angle < 270){ //Third quadrant
        vectorAngle = 90 - (270 - angle);
    }
    else if(angle > 270 && angle < 360){ //Fourth quadrant
        vectorAngle = 360 - angle;
    }

    return orientationVector = OrientationVector{sin(vectorAngle), cos(vectorAngle)};
}

/*!
    Returns angle relative to the direction the direction the person is facing.
*/
double findAngleToTree(const OrientationVector &currentOrientationVector, const Tree &targetTree, const Person &person){

    //substract each tree's coordinate from the person's coordinate to enure that the person is used as the origin.
    OrientationVector targetTreeOrientationVector = getOrientationUnitVector(targetTree.xCoordinate - person.xCoordinate, targetTree.yCoordinate - person.yCoordinate);
    double dotProduct = (targetTreeOrientationVector.xCoordinate * currentOrientationVector.xCoordinate + targetTreeOrientationVector.yCoordinate * currentOrientationVector.yCoordinate);
    double angle = acos(dotProduct) *  180 / PI; //acos(double) returns results expressed in radians. Therefore convert to degrees.
    return angle;
};



/*!
    converts user's each individual input inorder to utilized.
    Gives an error if the input is not a number and returns false.
    Returns true if the input has been converted to a number.
*/
bool convertStringToNumber(std::string &word, double &parameter){
    if(!std::any_of(word.begin(), word.end(), ::isdigit))
    {
        std::cout << "Error bad input. Use only numbers \nReformat previous input\n";
        return false;
    };

    double numberValue;
    std::string valueString;
    try
    {
        {
            numberValue = std::stod(word);
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    parameter = numberValue;
    return true;
};

/*!
    Parses user input to initialize a person based on the user's specified parameters.
*/
Person createPerson(){

    double xCoordinate = 0;
    double yCoordinate = 0;
    double fieldOfViewAngle = 0;
    std::string dataString;
    int fieldIndex = 0;
  
    /*
        Parses user input to all Person properties.
        Note: This should be merged with a similar block in fillMapWithTrees to form a separate function.
        This will loop untill all properties are initialized.
    */
    while(fieldIndex < 3){
        
        fieldIndex = 0;

        std::cout << "Please input person's coordinate in this format:\n <x coordinate> <y coordinate> <field of view angle not greater than 360>\n";
        std::getline(std::cin, dataString);
        

        std::stringstream ss(dataString);
        std::string word;
        
        
        while (ss >> word){
            switch (fieldIndex)
            {
            case 0:
                convertStringToNumber(word, xCoordinate) ? fieldIndex = 1 : fieldIndex = 0;
                break;
            
            case 1:
                convertStringToNumber(word, yCoordinate) ? fieldIndex = 2 : fieldIndex = 0;
                break;
            
            case 2:
                convertStringToNumber(word, fieldOfViewAngle) ? fieldIndex = 3 : fieldIndex = 0;
                break;
            
            default:
                break;
            }
        }
    }
    return Person{xCoordinate, yCoordinate, fieldOfViewAngle};
};

/*!
    Creates a number of trees based on parameters given by the user.
*/
std::vector<Tree> populateMapWithTrees(){
    std::vector<Tree> trees;
    std::string dataString;
    double numberOfTrees;

    double xCoordinate = 0;
    double yCoordinate = 0;
    
    std::cout << "Please fill in the desired number of trees:\n";
    std::cin >> numberOfTrees;
    std::cin.ignore();

    while(trees.size() < numberOfTrees)
    {   
        std::cout << "\n\n" << trees.size() << " out of " << numberOfTrees << " initialized";
        std::cout << "Please input next tree's coordinate in this format:\n <x coordinate> <y coordinate> \n";
        
        std::getline(std::cin, dataString);
        std::stringstream ss(dataString);
        std::string word;
        
        int fieldIndex = 0;
        while (ss >> word){
            switch (fieldIndex)
            {
            case 0:
                convertStringToNumber(word, xCoordinate) ? fieldIndex = 1 : fieldIndex = 0;
                    
                break;
            
            case 1:
                convertStringToNumber(word, yCoordinate) ? fieldIndex = 2 : fieldIndex = 0;

                break;
            default:
                break;
            }
        }
        if(fieldIndex < 2){
            std::cout << "Incompelete data. Ensure that both coordinates are filled in for previous tree.\n";
            continue;
        }
        trees.push_back(Tree{xCoordinate,yCoordinate});
    }

    return trees;
};

/*!
    Outputs the optimal orientation a person should be inorder to see the maximum amount of trees.
    This orientation is returned as unit vector.
*/
void calculateOptimalOrientation(const Person &person, const std::vector<Tree> &allTrees){

    OrientationVector optimalOrientation;
    int optimalAngle = 0;

    std::vector<Tree> maxSeenTrees;
    Tree currentReferenceTree;

    std::vector<Tree> currentSeenTrees;

    for(int angle = 0; angle <= 360; angle++){
        OrientationVector currentOrientationVector = getOrientationUnitVectorCoordinates(angle);
        currentSeenTrees = {}; // clear trees

        for(const Tree &tree : allTrees){
            double currentOrientationAngleToTree = findAngleToTree(currentOrientationVector, tree, person);
           
            if(person.fieldOfViewAngle / 2 > currentOrientationAngleToTree)
                currentSeenTrees.push_back(tree);

            if(currentSeenTrees.size() > maxSeenTrees.size()){
                maxSeenTrees = currentSeenTrees;
                optimalAngle = angle;
                optimalOrientation = currentOrientationVector;
            }
            if( angle == 270)
            {
                std::cout << "angle breaker reached";
            }
        }
        
    }

    std::cout << optimalOrientation.xCoordinate << "," << optimalOrientation.yCoordinate << " is the person's orientation as a unit vector pointing to desired viewing orientation\n";
    std::cout << "Optimal viewing angle: " << optimalAngle;
};

int main() {
    Person person = createPerson();
    std::vector<Tree> trees = populateMapWithTrees();
    calculateOptimalOrientation(person,trees);
    std::cin.get();
    return 0;
}