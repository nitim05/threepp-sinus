#include "threepp/threepp.hpp"
#include <vector>
#include <memory>
#include <cmath>



using namespace threepp;

int main() {
    // Vindu og rendering
    Canvas canvas("Sin wave animation");
    GLRenderer renderer(canvas);

    // Scene
    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    // Kamera
    auto camera = PerspectiveCamera::create(60, canvas.aspect(), 0.1f, 1000);
    camera->position.z = 10;

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
    auto ambientLight = AmbientLight::create(0xffffff, 0.4f); // hvitt lys med 40 % styrke

    // Parametre for boksene
    int numberOfBoxes =8;
    float spacing = 1.5f;
    float amplitude = 1.5f;
    float frequency = 0.5f;

    //laget liste av bokser og materialer for å holde styr på dem
    std::vector<std::shared_ptr<Mesh>>boxes;
    auto geometry = BoxGeometry::create(1, 1, 1);

    // Et delt material for hver gruppe
    auto evenMaterial = MeshStandardMaterial::create();
    evenMaterial->color = Color::blue;

    auto oddMaterial = MeshStandardMaterial::create();
    oddMaterial->color = Color::blue;

    // Lager en sentrerer ekke av bokser
        float center = (numberOfBoxes - 1) / 2.0f;

    for (int i = 0; i < numberOfBoxes; i++) {
        auto material = evenMaterial;

        if (i % 2 == 1) {
            material = oddMaterial;
        }

        auto box = Mesh::create(geometry, material);
        box->position.x = (i - center) * spacing;

        scene->add(box);
        boxes.push_back(box);
    }


    // Variabler for å holde styr på hvilken gruppe som skal endres
    int group = 0;
    bool evenGroupBlue = true;
    bool oddGroupBlue = true;

    canvas.onKeyPressed([&](KeyEvent event) {
        if (event.key == Key::SPACE) {
            if (group == 0) {
                if (evenGroupBlue) {
                    evenMaterial->color = Color::orange;
                } else {
                    evenMaterial->color = Color::blue;
                }

                evenGroupBlue = !evenGroupBlue;
                group = 1;
            } else {
                if (oddGroupBlue) {
                    oddMaterial->color = Color::red;
                } else {
                    oddMaterial->color = Color::blue;
                }

                oddGroupBlue = !oddGroupBlue;
                group = 0;
            }
        }
    });




    // animasjon av boksene mens de beveger seg
    Clock clock;

    canvas.animate([&] {
        float time = clock.getElapsedTime();

        for (int i = 0; i < numberOfBoxes; i++) {
            float phase = 0;

            if (i % 2 == 1) {
                phase = math::PI;
            }

            float angle = 2 * math::PI * frequency * time + phase;
            float height = amplitude * std::sin(angle);

            boxes[i]->position.y = height;
        }

        renderer.render(*scene, *camera);
    });


}
