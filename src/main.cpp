#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplan.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/normalshader.h"
#include "shaders/directshader.h"

#include "materials/phongmaterial.h"





void buildSceneSphere(Camera*& cam, Film*& film,
    std::vector<Shape*>*& objectsList,
    std::vector<PointLightSource>*& lightSourceList)
{
    /* **************************** */
      /* Declare and place the camera */
      /* **************************** */
      // By default, this gives an ID transform
      //  which means that the camera is located at (0, 0, 0)
      //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */
    // (...)
    Material *green_50 = new PhongMaterial (Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 1);


    /* ******* */
    /* Objects */
    /* ******* */
    // Create a heterogeneous list of objects of type shape
    // (some might be triangles, other spheres, plans, etc)
    objectsList = new std::vector<Shape*>;

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-1.0, -0.5, 2 * std::sqrt(2.0)));
    Shape* s1 = new Sphere(0.25, sphereTransform1, green_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.0, 0.0, 6));
    Shape* s2 = new Sphere(1, sphereTransform2, green_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(0.3, -0.75, 3.5));
    Shape* s3 = new Sphere(0.25, sphereTransform3, green_50);

    // Store the objects in the object list
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);


    /* ****** */
    /* Lights */
    /* ****** */
    
    // ADD YOUR LIGHT SOURCES HERE
    PointLightSource *l1 = new PointLightSource(Vector3D(-1, 2, 1), 5.00);
    //PointLightSource *l2 = new PointLightSource(Vector3D(10, 1, 1), 1.00);
    
    // DO NOT FORGET TO STORE THE LIGHT SOURCES IN THE "lightSourceList"
    lightSourceList = new std::vector<PointLightSource>;
    lightSourceList->push_back(*l1);
    //lightSourceList->push_back(*l2);
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor(cameraRay, *objectsList, *lightSourceList);

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}


//------------TASK 1---------------------//
void PaintImage(Film* film)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main Image Loop
    for (size_t y = 0; y < resY; y++)
    {
        // Show progression
        if (y % (resY / sizeBar) == 0)
            std::cout << ".";

        for (size_t x = 0; x < resX; x++)
        {
            //Task 1
            Vector3D random_color = Vector3D((x / (double)resX), (y / (double)resY), 0);
            film->setPixelValue(x, y, random_color);          
        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 512);


    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
    
    //Task 2
    //Shader *shader = new IntersectionShader (intersectionColor, bgColor);
    
    //Task 3 
    //Shader *shader = new DepthShader(intersectionColor, 8.00, bgColor);

    //Task 4
    //Shader *shader = new NormalShader();

    //Task 5
    Shader* shader = new DirectShader();
  

    // Build the scene---------------------------------------------------------
    // 
    // Declare pointers to all the variables which describe the scene
    Camera* cam;
    std::vector<Shape*>* objectsList;
    std::vector<PointLightSource>* lightSourceList;
    //Create Scene Geometry and Illumiantion
    buildSceneSphere(cam, film, objectsList, lightSourceList);

    //---------------------------------------------------------------------------

    //Paint Image ONLY TASK 1
    PaintImage(film);

    // Launch some rays! TASK 2,3,...
    // 
    raytrace(cam, shader, film, objectsList, lightSourceList);
    

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
