#include "threepp/threepp.hpp"
#include <vector>
#include <memory>
#include <cmath>



using namespace threepp;

int main() {
    // Vindu og rendering
    Canvas canvas("threepp");
    GLRenderer renderer(canvas);

    // Scene
    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    // Kamera
    auto camera = PerspectiveCamera::create(60, canvas.aspect(), 0.1f, 1000);
    camera->position.z = 5;

    // Endring av vindusstørrelse
    canvas.onWindowResize([&](WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    // Lys
    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);
    scene->add(AmbientLight::create(0xffffff, 0.4f)); // hvitt lys med 40 % styrke

    // Kube
  
        auto geometry = BoxGeometry::create(1, 1, 1);
        auto material = MeshStandardMaterial::create();
        material->color = Color::blue;
        auto box = Mesh::create(geometry, material);
        scene->add(box);


        // Animasjon
        Clock clock;
        canvas.animate([&] {
            const float dt = clock.getDelta();
            box->rotation.y += 1.0f * dt;// én radian per sekund
            renderer.render(*scene, *camera);
        });

}
