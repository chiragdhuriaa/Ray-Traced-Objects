# Ray-Tracing

![image](https://github.com/user-attachments/assets/19584c41-eecc-405b-949a-07852b624ac0)

About
This ray tracer is a basic yet enhanced rendering engine written in C++. It simulates the way light interacts with objects in a 3D scene to produce realistic images. The core concept of ray tracing is the simulation of light rays as they travel from a camera (or viewer) through a scene, intersecting with objects (such as spheres) and bouncing off surfaces to simulate reflection, refraction, and shading effects.

The project includes features such as:

    1. Basic Scene Rendering with spheres.
    2. Phong Shading Model for realistic lighting effects (ambient, diffuse, specular).
    3. Reflection and Refraction effects to simulate reflective and transparent materials.
    4. Scene Management where multiple spheres with various properties can be added.

Key Features

    1. Reflection: The ray tracer supports reflection on surfaces. When a ray hits a reflective object, a new ray is cast in the reflection direction, producing realistic shiny surfaces like mirrors or water.
    2. Refraction: The ray tracer simulates refraction for transparent materials such as glass or water. Using Snell's Law, rays passing through a refractive surface bend based on the material’s refractive index.
    3. Phong Shading: The shading model implemented includes:
        a. Ambient lighting: A constant light that illuminates all objects equally.
        b. Diffuse lighting: Light scattered in all directions after hitting a surface, dependent on the angle of incidence.
        c. Specular highlights: Bright spots of light created on shiny surfaces, simulating highlights from light sources.
    4. Scene Configuration: Users can define spheres with different properties, including color, reflectivity, and refractive index. Multiple spheres can be added to the scene to create a complex setup.
    5. Output Format: The output image is generated in the PPM (Portable PixMap) format, which can be viewed using any image viewer that supports this format.

How It Works

The ray tracing process is carried out in several steps:

    1. Ray Generation: Rays are cast from the camera (viewer's perspective) through each pixel on the screen.
    2. Ray-Object Intersection: Each ray is checked for intersections with objects in the scene (in this case, spheres). The nearest object that the ray intersects determines the pixel color.
    3. Lighting and Shading: Once an intersection is found, lighting calculations are performed, taking into account light sources, surface normals, and the material properties (like reflectivity and refraction).
    4. Reflection and Refraction: For reflective or refractive surfaces, additional rays are cast to simulate light bouncing off or passing through the object.

Technologies Used

    1. Programming Language: C++
    2. Libraries: Standard C++ libraries for input/output, file handling, and math (like <cmath> for geometric calculations).
    3. File Format: PPM (Portable PixMap) format for output images, which is easy to read and edit.

Project Goals

The goal of this project is to create a flexible, extensible ray tracer that can be used for learning and experimentation with ray tracing techniques. While it focuses on simplicity and readability, the project includes enough complexity to simulate various lighting effects and material properties.
Future Improvements

    1. Acceleration Structures: To speed up ray-object intersection tests, we can implement acceleration structures like Bounding Volume Hierarchies (BVH) or grids.
    2. Complex Materials: Adding more complex material models like glossy, transparent, and subsurface scattering materials.
    3. More Object Types: Support for other object types such as planes, cones, and triangles.
    4. Multi-threading: Implementing parallel processing to accelerate the ray tracing for larger scenes or higher resolution images.
    5. Image Output Formats: Supporting additional image formats (like PNG, BMP, etc.) for wider compatibility.

How to Run

    1. Clone or download the repository containing the project files.
    2. Open the project in a C++ IDE or text editor.
    3. Compile and run the program.
    4. The output will be saved in a file named output.ppm.
    5. Open the generated output.ppm file in an image viewer that supports the PPM format, or convert it to another image format if needed.
