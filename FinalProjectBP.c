#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define Max_USERS 100
#define Max_Name_Length 50
#define Max_Username_Length 50
#define Max_Password_Length 50
#define Max_Phone_Length 15
#define Max_Email_Length 50
#define Max_Words 1000
#define Max_Meanings 10
#define Max_Word_Length 50
#define Max_Meaning_Length 100
#define Max_Meanings 10
#define Max_Word_Length 50
#define Max_Meaning_Length 100


#define USERS_FILE "C:/Users/Parsa/CLionProjects/Project/users.txt"
#define ADMIN_FILE "C:/Users/Parsa/CLionProjects/Project/admin.txt"
#define DICTIONARY_FILE "C:/Users/Parsa/CLionProjects/Project/Dictionary.txt"


typedef struct {
    char word[Max_Word_Length];
    char meanings[Max_Meanings][Max_Meaning_Length];
    int meaningCount;
} Word;

struct LitnerBoxWord {
    Word word;
    struct LitnerBoxWord *next;
};

typedef struct {
    char firstName[Max_Name_Length];
    char lastName[Max_Name_Length];
    char username[Max_Username_Length];
    char password[Max_Password_Length];
    char phone[Max_Phone_Length];
    char email[Max_Email_Length];
    struct LitnerBoxWord* new;
    struct LitnerBoxWord* review;
    struct LitnerBoxWord* consolidation;
    struct LitnerBoxWord* learned;
    int leanrnedwordscount ;

} User;


typedef struct {
    Word words[Max_Words];
    int size;
} Dictionary;


typedef struct {
    User users[Max_USERS];
    int size;
    int currentUserIndex;  // Index of the currently signed-inn user
} UserDatabase;


typedef struct {
    Dictionary learnedwords;
} UserLearnedWords;


/////////////////
Dictionary dictionary = {0};
/////////////////


void adminMenu(UserDatabase *database);



void clear_screen() {
    system("cls");
}


void saveUsersToFile(UserDatabase *db) {
    FILE *file = fopen(USERS_FILE, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < db->size; i++) {
        fprintf(file, "%s %s %s %s %s %s\n", db->users[i].firstName, db->users[i].lastName, db->users[i].username,
                db->users[i].password, db->users[i].phone, db->users[i].email);
    }

    fclose(file);
}

void saveAdminToFile(Dictionary *dict) {
    FILE *file = fopen(ADMIN_FILE, "w");
    if (file == NULL) {
        printf("Error opening admin file for writing!\n");
        return;
    }

    for (int i = 0; i < dict->size; i++) {
        fprintf(file, "%s:", dict->words[i].word);
        for (int j = 0; j < dict->words[i].meaningCount; j++) {
            fprintf(file, " %s", dict->words[i].meanings[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void loadUsersFromFile(UserDatabase *db) {
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("No existing users found.\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s %s %s", db->users[db->size].firstName, db->users[db->size].lastName,
                  db->users[db->size].username, db->users[db->size].password, db->users[db->size].phone,
                  db->users[db->size].email) != EOF) {
        db->size++;
    }

    fclose(file);
}

void loadAdminFromFile() {
    FILE *file = fopen(DICTIONARY_FILE, "r");
    if (file == NULL) {
        printf("No admin words found.\n");
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), file)) {

        char *word_pos = strstr(line, "word: ");
        if (word_pos != NULL) {
            word_pos += strlen("word: ");
            char *end_word_pos = strstr(word_pos, ", translation:");
            if (end_word_pos != NULL) {
                *end_word_pos = '\0';
                char *word = word_pos;

                while (*word == ' ') word++;
                char *word_end = word + strlen(word) - 1;
                while (word_end > word && *word_end == ' ') word_end--;
                *(word_end + 1) = '\0';

                strcpy(dictionary.words[dictionary.size].word, word);
                dictionary.words[dictionary.size].meaningCount = 0;

                char *translation_pos = end_word_pos + strlen(", translation: ");
                char *meaning = strtok(translation_pos, "-\n");
                while (meaning != NULL) {
                    if (dictionary.words[dictionary.size].meaningCount >= Max_Meanings) {
                        printf("Too many meanings for word: %s\n", dictionary.words[dictionary.size].word);
                        break;
                    }

                    while (*meaning == ' ') meaning++;
                    char *meaning_end = meaning + strlen(meaning) - 1;
                    while (meaning_end > meaning && *meaning_end == ' ') meaning_end--;
                    *(meaning_end + 1) = '\0';

                    strcpy(dictionary.words[dictionary.size].meanings[dictionary.words[dictionary.size].meaningCount], meaning);
                    dictionary.words[dictionary.size].meaningCount++;

                    meaning = strtok(NULL, "-\n");
                }
                dictionary.size++;
                if (dictionary.size >= Max_Words) {
                    printf("Dictionary is full!\n");
                    break;
                }
            }
        }
    }

    fclose(file);
}


void signUp(UserDatabase *db) {
    if (db->size >= Max_USERS) {
        printf("User database is full!\n");
        return;
    }

    User newUser;
    printf("Enter first name: ");
    scanf("%s", newUser.firstName);
    printf("Enter last name: ");
    scanf("%s", newUser.lastName);
    printf("Enter username: ");
    scanf("%s", newUser.username);

    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->users[i].username, newUser.username) == 0) {
            printf("Username already exists!\n");
            return;
        }
    }

    printf("Enter password: ");
    scanf("%s", newUser.password);
    printf("Enter phone number: ");
    scanf("%s", newUser.phone);
    printf("Enter email: ");
    scanf("%s", newUser.email);

    db->users[db->size] = newUser;
    db->size++;

    printf("Sign up successful!\n");
    saveUsersToFile(db);  // Save users to file after signing up
}


void forgotPassword(UserDatabase *db) {
    char username[Max_Username_Length];
    char phone[Max_Phone_Length];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter phone number: ");
    scanf("%s", phone);

    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->users[i].username, username) == 0 && strcmp(db->users[i].phone, phone) == 0) {
            char newPassword[Max_Password_Length];
            printf("Enter new password: ");
            scanf("%s", newPassword);
            strcpy(db->users[i].password, newPassword);
            printf("Password reset successful!\n");
            saveUsersToFile(db);
            return;
        }
    }

    printf("Invalid username or phone number!\n");
}


void addWord() {
    if (dictionary.size >= Max_Words) {
        printf("Dictionary is full!\n");
        return;
    }

    Word newWord;
    printf("Enter word: ");
    scanf("%s", newWord.word);

    for (int i = 0; i < dictionary.size; i++) {
        if (strcmp(dictionary.words[i].word, newWord.word) == 0) {
            printf("Word already exists!\n");
            return;
        }
    }

    newWord.meaningCount = 0;
    int addMore;
    do {
        if (newWord.meaningCount >= Max_Meanings) {
            printf("Maximum meanings reached for this word!\n");
            break;
        }
        printf("Enter meaning: ");
        scanf(" %[^\n]", newWord.meanings[newWord.meaningCount]);
        newWord.meaningCount++;
        printf("Add another meaning? (1 for yes, 0 for no): ");
        scanf("%d", &addMore);
    } while (addMore);

    dictionary.words[dictionary.size++] = newWord;
    printf("Word added successfully!\n");
    saveAdminToFile(&dictionary);
    clear_screen();
}


void editWord() {
    char wordToEdit[Max_Word_Length];
    printf("Enter the word to edit: ");
    scanf("%s", wordToEdit);

    for (int i = 0; i < dictionary.size; i++) {
        if (strcmp(dictionary.words[i].word, wordToEdit) == 0) {
            int choice;
            do {
                printf("Editing '%s'\n", dictionary.words[i].word);
                printf("1. Add Meaning\n2. Remove Meaning\n3. Back\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1) {
                    if (dictionary.words[i].meaningCount >= Max_Meanings) {
                        printf("Maximum meanings reached for this word!\n");
                    } else {
                        printf("Enter new meaning: ");
                        scanf(" %[^\n]", dictionary.words[i].meanings[dictionary.words[i].meaningCount++]);
                        printf("Meaning added successfully!\n");
                    }
                } else if (choice == 2) {
                    int meaningIndex;
                    for (int j = 0; j < dictionary.words[i].meaningCount; j++) {
                        printf("%d. %s\n", j + 1, dictionary.words[i].meanings[j]);
                    }
                    printf("Enter the index of the meaning to remove: ");
                    scanf("%d", &meaningIndex);
                    if (meaningIndex > 0 && meaningIndex <= dictionary.words[i].meaningCount) {
                        for (int j = meaningIndex - 1; j < dictionary.words[i].meaningCount - 1; j++) {
                            strcpy(dictionary.words[i].meanings[j], dictionary.words[i].meanings[j + 1]);
                        }
                        dictionary.words[i].meaningCount--;
                        printf("Meaning removed successfully!\n");
                    } else {
                        printf("Invalid index!\n");
                    }
                    saveAdminToFile(&dictionary);
                }
            } while (choice != 3);
            return;
        }
    }
    printf("Word not found!\n");
    clear_screen();

}


void removeWord() {
    char wordToRemove[Max_Word_Length];
    printf("Enter the word to remove: ");
    scanf("%s", wordToRemove);

    for (int i = 0; i < dictionary.size; i++) {
        if (strcmp(dictionary.words[i].word, wordToRemove) == 0) {
            for (int j = i; j < dictionary.size - 1; j++) {
                dictionary.words[j] = dictionary.words[j + 1];
            }
            dictionary.size--;
            printf("Word removed successfully!\n");
            saveAdminToFile(&dictionary);
            return;
        }
    }
    printf("Word not found!\n");
    clear_screen();

}


void showAllWords() {
    for (int i = 0; i < dictionary.size - 1; i++) {
        for (int j = i + 1; j < dictionary.size; j++) {
            if (strcmp(dictionary.words[i].word, dictionary.words[j].word) > 0) {
                Word temp = dictionary.words[i];
                dictionary.words[i] = dictionary.words[j];
                dictionary.words[j] = temp;
            }
        }
    }

    for (int i = 0; i < dictionary.size; i++) {
        printf("%s:\n", dictionary.words[i].word);
        for (int j = 0; j < dictionary.words[i].meaningCount; j++) {
            printf("  - %s\n", dictionary.words[i].meanings[j]);
        }
    }
}


void readFromFile() {
    char filename[Max_Word_Length];
    printf("Enter the filename: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found!\n");
        return;
    }

    char line[200];  // Assuming no line is longer than 200 characters
    while (fgets(line, sizeof(line), file)) {

        // Check if the line contains "word:"
        char *word_pos = strstr(line, "word: ");
        if (word_pos != NULL) {
            // Extract the word
            word_pos += strlen("word: ");
            char *end_word_pos = strstr(word_pos, ", translation:");
            if (end_word_pos != NULL) {
                *end_word_pos = '\0';
                char *word = word_pos;

                // Remove leading and trailing spaces
                while (*word == ' ') word++;
                char *word_end = word + strlen(word) - 1;
                while (word_end > word && *word_end == ' ') word_end--;
                *(word_end + 1) = '\0';

                // Copy word into dictionary
                strcpy(dictionary.words[dictionary.size].word, word);
                dictionary.words[dictionary.size].meaningCount = 0;

                // Extract meanings
                char *translation_pos = end_word_pos + strlen(", translation: ");
                char *meaning = strtok(translation_pos, "-\n");
                while (meaning != NULL) {
                    if (dictionary.words[dictionary.size].meaningCount >= Max_Meanings) {
                        printf("Too many meanings for word: %s\n", dictionary.words[dictionary.size].word);
                        break;
                    }

                    // Remove leading and trailing spaces from meaning
                    while (*meaning == ' ') meaning++;
                    char *meaning_end = meaning + strlen(meaning) - 1;
                    while (meaning_end > meaning && *meaning_end == ' ') meaning_end--;
                    *(meaning_end + 1) = '\0';

                    // Add meaning to word in dictionary
                    strcpy(dictionary.words[dictionary.size].meanings[dictionary.words[dictionary.size].meaningCount],
                           meaning);
                    dictionary.words[dictionary.size].meaningCount++;

                    // Get next meaning
                    meaning = strtok(NULL, "-\n");
                }

                // Increment dictionary size
                dictionary.size++;
                if (dictionary.size >= Max_Words) {
                    printf("Dictionary is full!\n");
                    break;
                }
            }
        }
    }
    fclose(file);
    clear_screen();
}


void adminMenu(UserDatabase *database) {
    int choice;

    while (1) {
        printf("Admin Menu:\n");
        printf("1. Add Word\n");
        printf("2. Edit Word\n");
        printf("3. Remove Word\n");
        printf("4. Read From File\n");
        printf("5. Show All Words\n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addWord();
                break;
            case 2:
                editWord();
                break;
            case 3:
                removeWord();
                break;
            case 4:
                readFromFile();
                break;
            case 5:
                showAllWords();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}


int has_one_difference(const char *word1, const char *word2) {
    int length1 = strlen(word1);
    int length2 = strlen(word2);

    if (length1 != length2) {
        return 0;
    }

    int diff_count = 0;
    for (int i = 0; i < length1; i++) {
        if (word1[i] != word2[i]) {
            diff_count++;
            if (diff_count > 1) {
                return 0;
            }
        }
    }
    return diff_count == 1 ? 1 : 0;
}


void searching_new_words(User *user) {
    char str[Max_Word_Length];
    printf("Please Enter The Word To Find: ");
    scanf("%s", str);

    int found = 0;

    // Search for the word in the dictionary
    for (int i = 0; i < dictionary.size; i++) {
        if (strcmp(dictionary.words[i].word, str) == 0) {
            found = 1;
            printf("%s:\n", dictionary.words[i].word);
            for (int j = 0; j < dictionary.words[i].meaningCount; j++) {
                printf("%d. %s\n", j + 1, dictionary.words[i].meanings[j]);
            }

            printf("\nEnter + to add it to the Leitner box or = to add a meaning: ");
            char option;
            scanf(" %c", &option);

            if (option == '+') {
                // Add word to user Leitner box
                struct LitnerBoxWord *newWord = (struct LitnerBoxWord *) malloc(sizeof(struct LitnerBoxWord));
                newWord->word = dictionary.words[i];
                newWord->next = user->new;
                user->new = newWord;
                printf("Word '%s' added to the Leitner box!\n", dictionary.words[i].word);
            } else if (option == '=') {
                if (dictionary.words[i].meaningCount >= Max_Meanings) {
                    printf("Maximum meanings reached for this word!\n");
                } else {
                    printf("Enter the new meaning: ");
                    scanf(" %[^\n]", dictionary.words[i].meanings[dictionary.words[i].meaningCount]);
                    dictionary.words[i].meaningCount++;
                    printf("Meaning added successfully!\n");
                }
            } else {
                printf("Invalid option.\n");
            }

            break; // Exit loop once word is found
        }
    }

    if (!found) {
        // If word not found, suggest similar words
        printf("Word not found. Similar words:\n");
        for (int i = 0; i < dictionary.size; i++) {
            if (has_one_difference(str, dictionary.words[i].word) == 1) {
                printf("%s\n", dictionary.words[i].word);
            }
        }
    }
    clear_screen();
}


void delete_word(Dictionary *dict, int index) {
    // Shift words to fill the gap
    for (int i = index; i < dict->size - 1; i++) {
        dict->words[i] = dict->words[i + 1];
    }
    dict->size--;
}


void edit_meanings(Word *word) {
    clear_screen();
    printf("Editing meanings for: %s\n", word->word);
    printf("Enter new meanings (press Enter to finish):\n");

    // Clear existing meanings
    word->meaningCount = 0;

    // Read new meanings
    char new_meaning[Max_Meaning_Length];
    while (1) {
        printf("Meaning %d: ", word->meaningCount + 1);
        fgets(new_meaning, Max_Meaning_Length, stdin);
        new_meaning[strcspn(new_meaning, "\n")] = '\0';  // Remove newline character

        if (strlen(new_meaning) == 0) {
            break;  // Exit loop on empty input
        }

        strcpy(word->meanings[word->meaningCount], new_meaning);
        word->meaningCount++;

        if (word->meaningCount >= Max_Meanings) {
            printf("Maximum meanings reached.\n");
            break;
        }
    }
}

void iterateNewWords(User *user) {
    struct LitnerBoxWord *list = user->new;
    while (list != NULL) {
        printf("%s\n", list->word.word);
        printf("Choose one of the following options:\n");
        printf("1. Move to the next word\n");
        printf("2. show meanings\n");
        printf("3. Back\n");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            struct LitnerBoxWord *temp;
            case 1:
                // remove word from new list and add it to review list
                if (user->review == NULL) {
                    user->review = list;
                    list = list->next;
                } else {
                    temp = list;
                    list = list->next;
                    temp->next = user->review;
                    user->review = temp;
                }
                printf("Word moved to review list!\n");
                user->new = list;
                break;
            case 2:
                printf("Meanings:\n");
                for (int i = 0; i < list->word.meaningCount; i++) {
                    printf("%d. %s\n", i + 1, list->word.meanings[i]);
                }
                break;
            case 3:
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
    printf("No more new words to review!\n");
}


void iterateReviewWords(User *user) {
    struct LitnerBoxWord *list = user->review;
    while (list != NULL) {
        printf("%s\n", list->word.word);
        printf("Choose one of the following options:\n");
        printf("1. Move to the next word\n");
        printf("2. show meanings\n");
        printf("3. I know this word\n");
        printf("4. I don't know this word\n");
        printf("5. Back\n");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            struct LitnerBoxWord *temp;
            case 1:
                list = list->next;
                break;
            case 2:
                printf("Meanings:\n");
                for (int i = 0; i < list->word.meaningCount; i++) {
                    printf("%d. %s\n", i + 1, list->word.meanings[i]);
                }
                break;
            case 3:
                // remove word from review list and add it to consolidation list
                if (user->consolidation == NULL) {
                    user->consolidation = list;
                    list = list->next;
                } else {
                    temp = list;
                    list = list->next;
                    temp->next = user->consolidation;
                    user->consolidation = temp;
                }
                printf("Word moved to consolidation list!\n");
                user->review = list;
                break;
            case 4:
                list = list->next;
                break;
            case 5:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
    printf("No more review words to review!\n");
}

void iterateConsolidationWords(User *user) {
    struct LitnerBoxWord *list = user->consolidation;
    while (list != NULL) {
        printf("%s\n", list->word.word);
        printf("Choose one of the following options:\n");
        printf("1. Move to the next word\n");
        printf("2. show meanings\n");
        printf("3. I know this word\n");
        printf("4. I don't know this word\n");
        printf("5. Back\n");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            struct LitnerBoxWord *temp;
            case 1:
                list = list->next;
                break;
            case 2:
                printf("Meanings:\n");
                for (int i = 0; i < list->word.meaningCount; i++) {
                    printf("%d. %s\n", i + 1, list->word.meanings[i]);
                }
                break;
            case 3:
                user->leanrnedwordscount ++;
                // remove word from consolidation list and add it to learned list
                if (user->learned == NULL) {
                    user->learned = list;
                    list = list->next;
                } else {
                    temp = list;
                    list = list->next;
                    temp->next = user->learned;
                    user->learned = temp;
                }
                printf("Word moved to learned list!\n");
                user->consolidation = list;
                break;
            case 4:
                if (user->review == NULL) {
                    user->review = list;
                    list = list->next;
                } else {
                    temp = list;
                    list = list->next;
                    temp->next = user->review;
                    user->review = temp;
                }
                printf("Word moved to review list!\n");
                user->consolidation = list;
                break;
            case 5:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
    printf("No more consolidation words to review!\n");
}


void iterateLearnedWords(User *user) {
    struct LitnerBoxWord *list = user->learned;
    while (list != NULL) {
        printf("%s\n", list->word.word);
        printf("Choose one of the following options:\n");
        printf("1. Move to the next word\n");
        printf("2. show meanings\n");
        printf("3. Back\n");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                list = list->next;
                break;
            case 2:
                printf("Meanings:\n");
                for (int i = 0; i < list->word.meaningCount; i++) {
                    printf("%d. %s\n", i + 1, list->word.meanings[i]);
                }
                break;
            case 3:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
    printf("No more learned words to review!\n");
}


void litnerBoxMenu(User *user) {
    int choice;

    do {
        printf("\nLitner Box Menu\n");
        printf("1. View New Words\n");
        printf("2. View Review Words\n");
        printf("3. View Consolidation Words\n");
        printf("4. View Learned Words\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                iterateNewWords(user);
                break;
            case 2:
                iterateReviewWords(user);
                break;
            case 3:
                iterateConsolidationWords(user);
                break;
            case 4:
                iterateLearnedWords(user);
                break;
            case 5:
                return;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);
}



void createQuiz(User *user) {
    if (user->leanrnedwordscount== 0) {
        printf("Warning: You haven't learned any words yet!\n");
        return;
    }

    srand(time(0));
    int numQuestions = user->leanrnedwordscount < 10 ? user->leanrnedwordscount : 10;
    //user->learnedwords.size < 10: This condition checks if the user has learned fewer than 10 words.
    //? user->learnedwords.size : 10: This is the ternary operator. It works like an inline if-else statement:
    //If user->learnedwords.size < 10 is true, numQuestions is set to user->learnedwords.size.
    // If user->learnedwords.size < 10 is false, numQuestions is set to 10.


    printf("Quiz: Choose the correct meaning for each word\n\n");

    for (int i = 0; i < numQuestions; i++) {
        int correctIndex = rand() % 4;
        int wordIndex = rand() % user->leanrnedwordscount;
        Word *correctWord = (Word *) &user->learned[wordIndex];

        printf("Question %d: What is the meaning of the word '%s'?\n", i + 1, correctWord->word);

        char *options[4];
        int usedIndices[4] = {-1, -1, -1, -1};
        usedIndices[correctIndex] = wordIndex;

        options[correctIndex] = correctWord->meanings[rand() % correctWord->meaningCount];

        for (int j = 0; j < 4; j++) {
            if (j != correctIndex) {
                int randomWordIndex;
                do {
                    randomWordIndex = rand() % user->leanrnedwordscount;
                } while (randomWordIndex == wordIndex || usedIndices[0] == randomWordIndex ||
                         usedIndices[1] == randomWordIndex || usedIndices[2] == randomWordIndex ||
                         usedIndices[3] == randomWordIndex);
                usedIndices[j] = randomWordIndex;

                Word *randomWord = (Word *) &user->learned->word.word[randomWordIndex];
                options[j] = randomWord->meanings[rand() % randomWord->meaningCount];
            }
        }

        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, options[j]);
        }
        printf("\n");
    }
}


void showUserInformation(UserDatabase *db) {
    int currentUserIndex = db->currentUserIndex;
    if (currentUserIndex == -1) {
        printf("No user signed in!\n");
        return;
    }

    printf("User Information:\n");
    printf("First Name: %s\n", db->users[currentUserIndex].firstName);
    printf("Last Name: %s\n", db->users[currentUserIndex].lastName);
    printf("Username: %s\n", db->users[currentUserIndex].username);
    printf("Phone: %s\n", db->users[currentUserIndex].phone);
    printf("Email: %s\n", db->users[currentUserIndex].email);
    printf("\n");
}

void editInformation(UserDatabase *db) {
    int currentUserIndex = db->currentUserIndex;
    if (currentUserIndex == -1) {
        printf("No user signed in!\n");
        return;
    }

    printf("Editing User Information:\n");

    // Provide options to edit fields
    printf("1. Edit First Name (%s)\n", db->users[currentUserIndex].firstName);
    printf("2. Edit Last Name (%s)\n", db->users[currentUserIndex].lastName);
    printf("3. Edit Phone Number (%s)\n", db->users[currentUserIndex].phone);
    printf("4. Edit Email (%s)\n", db->users[currentUserIndex].email);
    printf("5. Back\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();  // Consume newline character

    switch (choice) {
        case 1:
            printf("Enter new first name: ");
            scanf("%s", db->users[currentUserIndex].firstName);
            printf("First name updated!\n");
            break;
        case 2:
            printf("Enter new last name: ");
            scanf("%s", db->users[currentUserIndex].lastName);
            printf("Last name updated!\n");
            break;
        case 3:
            printf("Enter new phone number: ");
            scanf("%s", db->users[currentUserIndex].phone);
            printf("Phone number updated!\n");
            break;
        case 4:
            printf("Enter new email: ");
            scanf("%s", db->users[currentUserIndex].email);
            printf("Email updated!\n");
            break;
        default:
            printf("Invalid choice!\n");
    }
}

void UserMenu(UserDatabase *usrdtbs) {
    while (1) {
        printf("Welcome To User Panel!\n");
        printf("Type What You Want\n1.Search\n2.LitnerBox\n3.Test\n4.ShowInformation\n5.EditInformation\n6.Back\n");

        int num2;
        scanf("%d", &num2);

        switch (num2) {
            case 1:
                searching_new_words(&usrdtbs->users[usrdtbs->currentUserIndex]);
                break;
            case 2: {
                litnerBoxMenu(&usrdtbs->users[usrdtbs->currentUserIndex]);
                break;
            }
            case 3: {
                createQuiz(usrdtbs->users);
                break;
            }
            case 4 : {
                showUserInformation(usrdtbs);
                break;
            }
            case 5 : {
                editInformation(usrdtbs);
                break;
            }
            default:
                return;
        }
    }
}


void UserOrAdmin(UserDatabase *usrdtbs) {
    char username_input[Max_Username_Length];
    char password_input[Max_Password_Length];

    printf("Please Enter Username!\n");
    scanf("%s", username_input);
    printf("Please Enter Password!\n");
    scanf("%s", password_input);

    for (int i = 0; i < usrdtbs->size; i++) {
        if (strcmp(usrdtbs->users[i].username, username_input) == 0 &&
            strcmp(usrdtbs->users[i].password, password_input) == 0) {
            usrdtbs->currentUserIndex = i;  // Set the current user index
            printf("Login Successful!\n");
            if (strcmp(username_input, "Admin") == 0 && strcmp(password_input, "Admin") == 0) {
                printf("Welcome To Admin panel\n");
                adminMenu(usrdtbs);
            } else {
                UserMenu(usrdtbs);
            }
            return;
        }
    }

    printf("Invalid username or password!\n");
}


int main() {
    UserDatabase database;
    database.size = 0;
    loadAdminFromFile();
    loadUsersFromFile(&database);
    printf("Welcome!\n");
    int choice;

    while (1) {
        printf("Menu:\n");
        printf("1. Sign Up\n");
        printf("2. Sign In\n");
        printf("3. Forgot Password\n");
        printf("4. Exit\n");
        printf("Enter your choice: \n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp(&database);
                break;
            case 2:
                UserOrAdmin(&database);
                break;
            case 3:
                forgotPassword(&database);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }

    }
}