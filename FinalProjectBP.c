#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
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


typedef struct {
    char firstName[Max_Name_Length];
    char lastName[Max_Name_Length];
    char username[Max_Username_Length];
    char password[Max_Password_Length];
    char phone[Max_Phone_Length];
    char email[Max_Email_Length];
} User;


typedef struct {
    char word[Max_Word_Length];
    char meanings[Max_Meanings][Max_Meaning_Length];
    int meaningCount;
} Word;


typedef struct {
    Word words[Max_Words];
    int size;
} Dictionary;


Dictionary newWords;
Dictionary reviewWords;
Dictionary consolidationWords;


typedef struct {
    User users[Max_USERS];
    int size;
} UserDatabase;


typedef struct ltrboxwords {
    Dictionary *boxwords;
    Word *word_info;
    struct ltrboxwords* next;
} ltrboxwords;


typedef struct {
    ltrboxwords* head;
} litnerbox_head;


typedef struct {
    Dictionary learnedwords;
} UserLearnedWords;


/////////////////


void LitnerFullPack(ltrboxwords* ltrboxwords1);
void adminMenu(UserDatabase* database);
void searching_new_words();





void clear_screen() {
    system("cls");
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
            return;
        }
    }

    printf("Invalid username or phone number!\n");
}




Dictionary dictionary = {0};


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
                    strcpy(dictionary.words[dictionary.size].meanings[dictionary.words[dictionary.size].meaningCount], meaning);
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


void adminMenu(UserDatabase* database) {
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


void initLeitnerBox(litnerbox_head *box) {
    box->head = NULL;
}


void addWordToLeitnerBox(litnerbox_head *box, Word *word) {
    ltrboxwords *newNode = (ltrboxwords *)malloc(sizeof(ltrboxwords));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->boxwords = (Dictionary *)word;
    newNode->next = box->head;
    box->head = newNode;
    printf("Word '%s' added to the Leitner box!\n", word->word);
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


void searching_new_words() {
    litnerbox_head box;
    initLeitnerBox(&box); // Initialize Leitner box

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
                addWordToLeitnerBox(&box, &dictionary.words[i]);
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


void delete_word(Dictionary* dict, int index) {
    // Shift words to fill the gap
    for (int i = index; i < dict->size - 1; i++) {
        dict->words[i] = dict->words[i + 1];
    }
    dict->size--;
}


void edit_meanings(Word* word) {
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


void LitnerFullPack(ltrboxwords* ltrboxwords1) {
    for (int i = 0; i < ltrboxwords1->boxwords->size; i++) {
        clear_screen();
        printf("%s\n",ltrboxwords1->word_info[i].word);
        printf("Press 'Space' to see the meaning...\n");
        while (getch() != ' ') {
            for (int counter = 0; counter < ltrboxwords1->word_info->meaningCount; i++)
                printf("Meaning %d: %s\n", i + 1, dictionary.words[i].meanings[i]);
        }
        printf("Press 'Enter' to move to the next word...\n");
        while (getch() != '\r'); // Wait for Enter key
    }
    for (int i = 0 ; i < ltrboxwords1->boxwords->size; i++){
        clear_screen();
        printf("Review Part!\n");
        printf("%s\n",ltrboxwords1->word_info[i].word);
        printf("You Have 2 Options\n1.I have already known the word\n2.I did not know it\n3.Back\n");
        int num;
        scanf("%d",&num);
        switch (num) {
            // Move word to consolidation box
            case 1 :
            consolidationWords.words[consolidationWords.size++] = reviewWords.words[i];
            // Remove the word from the review box
            for (int j = i; j < reviewWords.size - 1; j++) {
                reviewWords.words[j] = reviewWords.words[j + 1];
            }
            reviewWords.size--;
            i--;  // Adjust index after removal
            case 2 :
                break;
            case 3:
                // Go back to the previous menu or exit review
                return;
        }
    }
    for (int i = 0; i < consolidationWords.size; i++) {
        clear_screen();
        printf("Consolidation Part!\n");
        printf("%s\n", consolidationWords.words[i].word);
        printf("You Have 2 Options\n1. I didn't know the word\n2. I learned the word\n3. Back\n");

        int choice;
        while ((choice = getch()) != '1' && choice != '2' && choice != '3');

        switch (choice) {
            case '1':
                // Move back to the review box
                reviewWords.words[reviewWords.size++] = consolidationWords.words[i];
                // Remove the word from the consolidation box
                for (int j = i; j < consolidationWords.size - 1; j++) {
                    consolidationWords.words[j] = consolidationWords.words[j + 1];
                }
                consolidationWords.size--;
                i--;  // Adjust index after removal
                break;
            case '2':
                // Move word to the next box (if applicable)
                for (int j = i; j < consolidationWords.size - 1; j++) {
                    consolidationWords.words[j] = consolidationWords.words[j + 1];
                }
                consolidationWords.size--;
                i--;  // Adjust index after removal
                break;
            case '3':
                // Go back to the previous menu or exit consolidation
                return;
        }
    }
    int choice;
    do {
        clear_screen();
        printf("Learned Words Review Part!\n");

        // Display learned words
        Dictionary learnedwords;
        for (int i = 0; i < learnedwords.size; i++) {
            printf("%d. %s\n", i + 1, learnedwords.words[i].word);
        }

        printf("Choose a word to review (Enter number), or '0' to go back: ");
        scanf("%d", &choice);
        getchar();  // Consume newline character

        if (choice >= 1 && choice <= learnedwords.size) {
            // Display meanings and options for the chosen word
            clear_screen();
            Word* chosenWord = &learnedwords.words[choice - 1];
            printf("Word: %s\n", chosenWord->word);
            for (int j = 0; j < chosenWord->meaningCount; j++) {
                printf("Meaning %d: %s\n", j + 1, chosenWord->meanings[j]);
            }
            printf("\nOptions:\n");
            printf("1. Edit meanings\n");
            printf("2. Delete word\n");
            printf("3. Back\n");

            while (1) {
                char option = getchar();
                getchar();  // Consume newline character

                switch (option) {
                    case '1':
                        // Edit meanings of the word
                        edit_meanings(chosenWord);
                        break;
                    case '2':
                        // Delete word from the learned words box
                        delete_word(&learnedwords, choice - 1);
                        break;
                    case '3':
                        // Go back to the previous menu
                        break;
                    default:
                        printf("Invalid option.\n");
                }

                if (option == '3')
                    break;
            }
        }
    } while (choice != 0);
    clear_screen();

}


void createQuiz(UserLearnedWords *user) {
    if (user->learnedwords.size == 0) {
        printf("Warning: You haven't learned any words yet!\n");
        return;
    }

    srand(time(0));
    int numQuestions = user->learnedwords.size < 10 ? user->learnedwords.size : 10;
    //user->learnedwords.size < 10: This condition checks if the user has learned fewer than 10 words.
    //? user->learnedwords.size : 10: This is the ternary operator. It works like an inline if-else statement:
    //If user->learnedwords.size < 10 is true, numQuestions is set to user->learnedwords.size.
    // If user->learnedwords.size < 10 is false, numQuestions is set to 10.


    printf("Quiz: Choose the correct meaning for each word\n\n");

    for (int i = 0; i < numQuestions; i++) {
        int correctIndex = rand() % 4;
        int wordIndex = rand() % user->learnedwords.size;
        Word *correctWord = &user->learnedwords.words[wordIndex];

        printf("Question %d: What is the meaning of the word '%s'?\n", i + 1, correctWord->word);

        char *options[4];
        int usedIndices[4] = { -1, -1, -1, -1 };
        usedIndices[correctIndex] = wordIndex;

        options[correctIndex] = correctWord->meanings[rand() % correctWord->meaningCount];

        for (int j = 0; j < 4; j++) {
            if (j != correctIndex) {
                int randomWordIndex;
                do {
                    randomWordIndex = rand() % user->learnedwords.size;
                } while (randomWordIndex == wordIndex || usedIndices[0] == randomWordIndex || usedIndices[1] == randomWordIndex || usedIndices[2] == randomWordIndex || usedIndices[3] == randomWordIndex);
                usedIndices[j] = randomWordIndex;

                Word *randomWord = &user->learnedwords.words[randomWordIndex];
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
    printf("User Information:\n");

    for (int i = 0; i < db->size; i++) {
        printf("User %d:\n", i + 1);
        printf("  First Name: %s\n", db->users[i].firstName);
        printf("  Last Name: %s\n", db->users[i].lastName);
        printf("  Username: %s\n", db->users[i].username);
        printf("  Phone: %s\n", db->users[i].phone);
        printf("  Email: %s\n", db->users[i].email);
        printf("\n");
    }
}


void UserMenu(UserDatabase* usrdtbs,UserLearnedWords*  lrndwrd){
    ltrboxwords *ltrltr = nullptr;
    printf("Welcome To User Panel!\n");
    printf("Type What You Want\n1.Search\n2.LitnerBox\n3.Test\n4.ShowInformation\n5.EditInformation\n6.Back\n");

    int num2;
    scanf("%d", &num2);

    switch (num2) {
        case 1:
            searching_new_words();
            break;
        case 2: {
            LitnerFullPack(ltrltr);
            break;
        }
        case 3: {
            createQuiz(lrndwrd);
            break;
        }
        case 4 :{
            showUserInformation(usrdtbs);
            break;
        }
        default:
            break;
    }

}


void UserOrAdmin(UserDatabase* usrdtbs,ltrboxwords* ltrltr, UserLearnedWords* lrndwrd){
    char username_input[Max_Username_Length];
    char password_input[Max_Password_Length];

    printf("Please Enter Username!\n");
    scanf("%s", username_input);
    printf("Please Enter Password!\n");
    scanf("%s", password_input);

    if (strcmp(username_input, "Admin") == 0 && strcmp(password_input, "Admin") == 0) {
        printf("Welcome To Admin panel\n");
        adminMenu(usrdtbs);
    } else {
        printf("Welcome To User panel\n");
        UserMenu(usrdtbs,lrndwrd);
    }
}






int main() {
    litnerbox_head box;
    initLeitnerBox(&box);
    UserDatabase database;
    database.size = 0;
    ltrboxwords ltrltr ;
    UserLearnedWords *learnedWords = nullptr;
    printf("Welcome!\n");
    int choice;

    while (1) {
        printf("Menu:\n");
        printf("1. Sign Up\n");
        printf("2. Sign In\n");
        printf("3. Forgot Password\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp(&database);
                break;
            case 2:
                UserOrAdmin(&database,&ltrltr,learnedWords);
                break;
            case 3:
                forgotPassword(&database);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }

    }
}
