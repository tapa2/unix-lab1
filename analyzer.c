#include <stdio.h>
#include <stdlib.h>

// Функція, яка малює одну смужку графіка у файл SVG
void draw_svg_bar(FILE *f, int y, char *label, long long count, double percent) {
    int max_width = 500; // Максимальна довжина смужки в пікселях
    int bar_width = (int)(max_width * (percent / 100.0));
    
    // Текст категорії (зліва)
    fprintf(f, "<text x=\"10\" y=\"%d\" font-family=\"sans-serif\" font-size=\"14\" fill=\"#333\">%s</text>\n", y + 15, label);
    
    // Кольорова смужка
    fprintf(f, "<rect x=\"120\" y=\"%d\" width=\"%d\" height=\"22\" fill=\"#4CAF50\" rx=\"3\" />\n", y, bar_width);
    
    // Текст зі значенням і відсотком (справа від смужки)
    fprintf(f, "<text x=\"%d\" y=\"%d\" font-family=\"sans-serif\" font-size=\"14\" fill=\"#333\">%lld (%.2f%%)</text>\n", 120 + bar_width + 10, y + 15, count, percent);
}

int main() {
    FILE *file = fopen("sizes.txt", "r");
    if (!file) {
        perror("Помилка: не вдалося відкрити sizes.txt");
        return 1;
    }

    long long count_0_1k = 0, count_1_10k = 0, count_10_100k = 0;
    long long count_100k_1m = 0, count_1m_10m = 0, count_10m_plus = 0;
    long long total_files = 0, size;

    // Зчитуємо дані
    while (fscanf(file, "%lld", &size) != EOF) {
        if (size <= 1024) count_0_1k++;
        else if (size <= 10240) count_1_10k++;
        else if (size <= 102400) count_10_100k++;
        else if (size <= 1048576) count_100k_1m++;
        else if (size <= 10485760) count_1m_10m++;
        else count_10m_plus++;
        total_files++;
    }
    fclose(file);

    if (total_files == 0) {
        printf("Файл sizes.txt порожній або не містить даних.\n");
        return 1;
    }

    // СТВОРЮЄМО ФАЙЛ З КАРТИНКОЮ
    FILE *svg = fopen("histogram.svg", "w");
    if (!svg) {
        perror("Помилка при створенні файлу графіка");
        return 1;
    }

    // Записуємо шапку SVG-файлу
    fprintf(svg, "<svg width=\"850\" height=\"280\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    fprintf(svg, "<rect width=\"100%%\" height=\"100%%\" fill=\"#f4f4f9\" rx=\"8\" />\n");
    fprintf(svg, "<text x=\"10\" y=\"30\" font-family=\"sans-serif\" font-size=\"18\" font-weight=\"bold\" fill=\"#2c3e50\">Частотний розподіл розміру файлів у системі</text>\n");

    // Малюємо смужки для кожної категорії
    draw_svg_bar(svg, 60, "0 - 1 KB", count_0_1k, (double)count_0_1k/total_files*100);
    draw_svg_bar(svg, 95, "1 - 10 KB", count_1_10k, (double)count_1_10k/total_files*100);
    draw_svg_bar(svg, 130, "10 - 100 KB", count_10_100k, (double)count_10_100k/total_files*100);
    draw_svg_bar(svg, 165, "100 KB - 1 MB", count_100k_1m, (double)count_100k_1m/total_files*100);
    draw_svg_bar(svg, 200, "1 MB - 10 MB", count_1m_10m, (double)count_1m_10m/total_files*100);
    draw_svg_bar(svg, 235, "> 10 MB", count_10m_plus, (double)count_10m_plus/total_files*100);

    // Закриваємо тег SVG
    fprintf(svg, "</svg>\n");
    fclose(svg);

    // Виводимо повідомлення у термінал
    printf("--- Аналіз успішно завершено! ---\n");
    printf("Всього проаналізовано файлів: %lld\n", total_files);
    printf("Згенеровано графік: histogram.svg\n");

    return 0;
}
