
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

struct Person{
    double xCoordinate;
    double yCoordinate;
    double fieldOfViewAngle; //Note: obviously should not be greater than 360
};

struct Tree{
    double xCoordinate;
    double yCoordinate;
};

struct UnitVector{
    double xCoordinate;
    double yCoordinate;
};

/*!
    Returns a unit vector
*/
UnitVector getUnitVector(const double &xCoordinate, const double &yCoordinate)
{
    UnitVector unitVector;
    double hypotenuse = 0;
    hypotenuse = std::sqrt(xCoordinate*xCoordinate + yCoordinate*yCoordinate);
    unitVector = UnitVector{xCoordinate/hypotenuse , yCoordinate/hypotenuse};
    return unitVector;
}

/*!
    Returns angle relative to the direction the direction the person is facing.
*/
double findAngleToTree(const Tree &targetTree, const Tree &referencTree, const Person &person){

    //substract each tree's coordinate from the person's coordinate to enure that the person is used as the origin.
    UnitVector targetTreeUnitVector = getUnitVector(targetTree.xCoordinate - person.xCoordinate, targetTree.yCoordinate - person.yCoordinate);
    UnitVector referenceUnitTreeVector = getUnitVector(referencTree.xCoordinate - person.xCoordinate, referencTree.yCoordinate - person.yCoordinate);
    double dotProduct = (targetTreeUnitVector.xCoordinate * referenceUnitTreeVector.xCoordinate + targetTreeUnitVector.yCoordinate * referenceUnitTreeVector.yCoordinate);
    double angle = acos(dotProduct);
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

    UnitVector optimalOrientation;
    Tree optimalTree;

    std::vector<Tree> maxSeenTrees;
    Tree currentReferenceTree;

    std::vector<Tree> currentSeenTrees;

    for(const Tree &currentIndexTree : allTrees){
        currentReferenceTree = currentIndexTree;
        for(const Tree &nextCurrentIndexTree : allTrees)
        {
            double currentTreeVectorAngle = findAngleToTree(currentReferenceTree, nextCurrentIndexTree, person);
            if(person.fieldOfViewAngle / 2 > currentTreeVectorAngle)
                currentSeenTrees.push_back(nextCurrentIndexTree);
        }
        if(currentSeenTrees.size() > maxSeenTrees.size()){
            maxSeenTrees = currentSeenTrees;
            optimalTree = currentReferenceTree;
        }
    }

    optimalOrientation = getUnitVector(optimalTree.xCoordinate,optimalTree.yCoordinate);

    std::cout << optimalOrientation.xCoordinate << "," << optimalOrientation.yCoordinate << " is the person's orientation as a unit vector pointing to desired viewing orientation";
        /*
         to-be-completed: read< some prerequisites on vectors or/and barycentric coordinates.
        */

};

int main() {
    Person person = createPerson();
    std::vector<Tree> trees = populateMapWithTrees();
    calculateOptimalOrientation(person,trees);
    std::cin.get();
    return 0;
}