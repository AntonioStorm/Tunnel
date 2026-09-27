#include <gpiod.h>
#include <iostream>
#include <unistd.h>

int main() {
    const char *chip_path = "/dev/gpiochip0";
    unsigned int line_offset = 17;

    // Abre o chip
    struct gpiod_chip *chip = gpiod_chip_open(chip_path);
    if (!chip) {
        std::cerr << "Erro ao abrir o chip GPIO\n";
        return 1;
    }

    // Configura as settings da linha (direção: entrada)
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);

    // Monta a configuração das linhas, associando o offset às settings
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    unsigned int offsets[] = { line_offset };
    gpiod_line_config_add_line_settings(line_cfg, offsets, 1, settings);

    // Configura a "request" (nome do consumidor, etc.)
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "leitura_gpio");

    // Faz a requisição da linha ao chip
    struct gpiod_line_request *request =
        gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    if (!request) {
        std::cerr << "Erro ao requisitar a linha GPIO\n";
        return 1;
    }

    // Loop de leitura
    while (true) {
        int valor = gpiod_line_request_get_value(request, line_offset);
        std::cout << (valor == GPIOD_LINE_VALUE_ACTIVE ? "HIGH" : "LOW") << std::endl;
        usleep(500000); // 0.5s
    }

    // Limpeza (nunca alcançado no loop infinito, mas fica documentado)
    gpiod_line_request_release(request);
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return 0;
}