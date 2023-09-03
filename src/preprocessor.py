class Preprocessor:

    ## Tokens
    tokens = ["include",
              "define",
              "if",
              "else",
              "elif",
              "fi",
              "endif",
              "ifdef",
              "ifndef",
              ]

    ## Remove all comments from the given text
    def strip_comments(self) {

    }

    #if defined(...) -> #ifdef
    #if !defined(...) -> #ifndef

    ## Initialise preprocessor instance
    def __init__(self, fileName: str):

        ## Save source file path
        self.sourcePath = fileName

        ## Read in text
        with open(fileName, 'r', encoding='unicode_escape') as f:
            self.sourceText = f.read()
    
    ## Handle #include

    ## Handle backslashes at EOL

    ## Handle #define



