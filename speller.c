/**
 * speller.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a spell-checker.
 */

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"
#undef calculate //сброс значения переменной  (счетчики)
#undef getrusage //сброс значения переменной (счетчики)

// default dictionary
#define DICTIONARY "dictionaries/large" //объявление константы словаря

// prototype
double calculate(const struct rusage* b, const struct rusage* a); //вызов функции счетчика (struct rusage - тип переменной, используемый для подсчета ресурсов затрачиваемых на процесс)

int main(int argc, char* argv[])// запуск основной на аргументах командной строки
{

    // check for correct number of args
    if (argc != 2 && argc != 3) //проверка аргументов командной строки
    {
        printf("Usage: speller [dictionary] text\n");
        return 1;
    }

    // structs for timing data
    struct rusage before, after;//??????

    // benchmarks
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // determine dictionary to use
    char* dictionary = (argc == 3) ? argv[1] : DICTIONARY;//? - тернарный оператор (заменяет if/else). Если argc==3, то dictionary=argv[1], иначе словарю из объявленной константы

    // load dictionary
    getrusage(RUSAGE_SELF, &before); //подсчет ресурсов для работы программы ДО загрузки словаря
    bool loaded = load(dictionary); //!!!!!!!!!!!!!!!!!!!!!!!!!!!
    getrusage(RUSAGE_SELF, &after);//подсчет ресурсов для работы программы ПОСЛЕ загрузки словаря

    // abort if dictionary not loaded
    if (!loaded) //проверка на удачную загрузку словаря
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // calculate time to load dictionary
    time_load = calculate(&before, &after); //подсчет времени загрузки (разница использования ресурсов ДО загрузки словаря с ПОСЛЕ загрузки)

    // try to open text
    char *text = (argc == 3) ? argv[2] : argv[1];//попытка открыть текст, если условие выполняется то argv[2], иначе argv[1] (если словарь загружается заявленный в константе)
    FILE* fp = fopen(text, "r");//открытие файла с текстом
    if (fp == NULL)             //проверка, открылся ли файл
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n"); //сообщение о словах с ошибкой

    // prepare to spell-check
    int index = 0, misspellings = 0, words = 0; //создание переменных для подсчета слов и ошибок
    char word[LENGTH+1]; // константа LENGHT объявлена в файле dictionary.h и равна 45

    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp)) //fgetc(fp) - получение символа из данного указателя позиции, EOF - результат, возвращаемый fgetc(fp) при окончании файла
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0)) //если символ из словаря ИЛИ символ конца строки и индекс>0
        {
             // append character to word
            word[index] = c;                            //добавление символа к массиву "word" c индексом [index]
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)                                     //игнорирование слишком длинных слов
            {

                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c)); //пока символ в словаре и не равен EOF - индекс символа 0

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c)) //игнорирование чисел
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));  //если символ число и пока символ не EOF, индекс =0

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';         // символ окончания слова

            // update counter
            words++;                //увеличиваем счетчик слов на 1

            // check word's spelling

            getrusage(RUSAGE_SELF, &before);    //подсчет ресурсов для работы программы ДО проверки слова
            bool misspelled = !check(word);    //!!!!!!!!!!!!!!!!!
            getrusage(RUSAGE_SELF, &after);     //подсчет ресурсов для работы программы ПОСЛЕ проверки слова

            // update benchmark
            time_check += calculate(&before, &after); //обновление счетчика времени

            // print word if misspelled
            if (misspelled)
            {
                printf("%s\n", word); // вывод слова с ошибкой (которого нет в словаре)
                misspellings++;
            }

            // prepare for next word
            index = 0; //подготовка к следующем слову
        }
    }

    // check whether there was an error
    if (ferror(fp))                 //проверка на ошибку потока данных из файла
    {
        fclose(fp);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // close text
    fclose(fp);

    // determine dictionary's size
    getrusage(RUSAGE_SELF, &before); //счетчик ДО
    unsigned int n = size();  //!!!!!!!!!!!!!!!!!!!!! подсчет размера словаря (слов)
    getrusage(RUSAGE_SELF, &after);                     //счетчик ПОСЛЕ

    // calculate time to determine dictionary's size
    time_size = calculate(&before, &after);              //счетчик времени подсчета объема словаря

    // unload dictionary
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();                           //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!загрузка словаря
    getrusage(RUSAGE_SELF, &after);

    // abort if dictionary not unloaded
    if (!unloaded)  //проверка на возможность загрузки словаря
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // calculate time to unload dictionary
    time_unload = calculate(&before, &after);   //блок счетчиков

    // report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
     time_load + time_check + time_size + time_unload);

    // that's all folks
    return 0;
}

/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}
