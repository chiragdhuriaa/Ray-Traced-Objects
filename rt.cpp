#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <limits>

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 normalize() const {
        float length = std::sqrt(x * x + y * y + z * z);
        return Vec3(x / length, y / length, z / length);
    }

    float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3 reflect(const Vec3& normal) const {
        return *this - normal * (2 * this->dot(normal));
    }

    Vec3 refract(const Vec3& normal, float refractiveIndex) const {
        float cosI = -normal.dot(*this);
        float sinT2 = refractiveIndex * refractiveIndex * (1 - cosI * cosI);
        if (sinT2 > 1) return Vec3(0, 0, 0); 
        float cosT = std::sqrt(1 - sinT2);
        return *this * refractiveIndex + normal * (refractiveIndex * cosI - cosT);
    }
};

class Ray {
public:
    Vec3 origin, direction;

    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalize()) {}
};

class Sphere {
public:
    Vec3 center;
    float radius;
    Vec3 color;
    float reflectivity;
    float refractiveIndex; 

    Sphere(const Vec3& c, float r, const Vec3& col, float refl, float refrIndex = 1.0f)
        : center(c), radius(r), color(col), reflectivity(refl), refractiveIndex(refrIndex) {}

    bool intersect(const Ray& ray, float& t) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;
        t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        return t > 0;
    }
};

class Scene {
public:
    std::vector<Sphere> spheres;

    void addSphere(const Sphere& sphere) {
        spheres.push_back(sphere);
    }
};

Vec3 calculateColor(const Ray& ray, const Scene& scene, const Vec3& lightPos, int depth = 0) {
    float nearestT = std::numeric_limits<float>::max();
    const Sphere* hitSphere = nullptr;

    for (const Sphere& sphere : scene.spheres) {
        float t;
        if (sphere.intersect(ray, t) && t < nearestT) {
            nearestT = t;
            hitSphere = &sphere;
        }
    }

    if (hitSphere) {
        Vec3 hitPoint = ray.origin + ray.direction * nearestT;
        Vec3 normal = (hitPoint - hitSphere->center).normalize();
        Vec3 lightDir = (lightPos - hitPoint).normalize();

        Vec3 ambient = hitSphere->color * 0.1f; 
        float diffuseFactor = std::max(0.0f, normal.dot(lightDir));
        Vec3 diffuse = hitSphere->color * diffuseFactor; 

        Vec3 viewDir = (ray.origin - hitPoint).normalize();
        Vec3 reflectDir = lightDir.reflect(normal);
        float specularFactor = std::pow(std::max(0.0f, viewDir.dot(reflectDir)), 32);
        Vec3 specular = Vec3(1.0f, 1.0f, 1.0f) * specularFactor; 

        Vec3 color = ambient + diffuse + specular;

        if (depth < 2 && hitSphere->reflectivity > 0) {
            Vec3 reflectionDir = ray.direction.reflect(normal);
            Ray reflectionRay(hitPoint + normal * 0.001f, reflectionDir);
            Vec3 reflectionColor = calculateColor(reflectionRay, scene, lightPos, depth + 1);
            color = color * (1 - hitSphere->reflectivity) + reflectionColor * hitSphere->reflectivity;
        }
        if (depth < 2 && hitSphere->refractiveIndex > 1.0f) {
            Vec3 refractionDir = ray.direction.refract(normal, 1.0f / hitSphere->refractiveIndex);
            if (refractionDir.x != 0 || refractionDir.y != 0 || refractionDir.z != 0) {
                Ray refractionRay(hitPoint - normal * 0.001f, refractionDir);
                Vec3 refractionColor = calculateColor(refractionRay, scene, lightPos, depth + 1);
                color = color * (1 - hitSphere->reflectivity) + refractionColor * hitSphere->reflectivity;
            }
        }

        return color;
    }

    return Vec3(0.2f, 0.2f, 0.3f); 
}

void render(Scene& scene, int width, int height) {
    std::vector<Vec3> image(width * height);

    Vec3 lightPos(5, 5, -5); 

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            float nx = (float)x / (width - 1) * 2 - 1; 
            float ny = (float)y / (height - 1) * 2 - 1; 
            Ray ray(Vec3(0, 0, 0), Vec3(nx, ny, -1)); 

            Vec3 color = calculateColor(ray, scene, lightPos);
            image[y * width + x] = color;
        }
    }

    std::ofstream ofs("output.ppm");
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (const Vec3& pixel : image) {
        ofs << (int)(std::min(1.0f, pixel.x) * 255) << " "
            << (int)(std::min(1.0f, pixel.y) * 255) << " "
            << (int)(std::min(1.0f, pixel.z) * 255) << "\n";
    }
}

int main() {
    Scene scene;
    scene.addSphere(Sphere(Vec3(0, 0, -5), 1, Vec3(1, 0, 0), 0.5f)); 
    scene.addSphere(Sphere(Vec3(2, 0, -5), 1, Vec3(0, 1, 0), 0.5f, 1.5f)); 
    scene.addSphere(Sphere(Vec3(-2, 0, -5), 1, Vec3(0, 0, 1), 0.5f)); 

    int width = 800, height = 600;
    render(scene, width, height);

    return 0;
}
