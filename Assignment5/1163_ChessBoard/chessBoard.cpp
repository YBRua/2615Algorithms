#include <iostream>
#define MAX_STATES 64
#define CONST 1000000007

using namespace std;

int result[201][MAX_STATES + 1][MAX_STATES + 1] = {0};
int broken[201] = {0};

// warning: hard-coding!
int firstRowConflictTable[MAX_STATES + 1] = {0};
int firstRowConflicts[6] = {
    4,
    8,
    17,
    34,
    4,
    8};
int secondRowConflictTable[MAX_STATES + 1] = {0};
int secondRowConflicts[6] = {
    2,
    5,
    10,
    20,
    40,
    16};

inline int mask(int msk, int argument)
{
    return msk & argument;
}

void initializeConflictTables(int n, int msk, int totalStates)
{
    for (int state = 0; state < totalStates; ++state)
    {
        for (int idx = 0, p = 1; idx < n; ++idx, p *= 2)
        {
            if (state & p)
            {
                firstRowConflictTable[state] = mask(msk, firstRowConflicts[idx] | firstRowConflictTable[state]);
                secondRowConflictTable[state] = mask(msk, secondRowConflicts[idx] | secondRowConflictTable[state]);
            }
        }
        /* for debugging purposes
        cout << "State: " << state << endl;
        cout << "Conflicts: " << endl;
        cout << "  - " << firstRowConflictTable[state] << endl;
        cout << "  - " << secondRowConflictTable[state] << endl;
        cout << endl;
        */
    }
}

inline void addBrokenCell(int row, int column)
{
    int location = 1 << (column - 1);
    broken[row] |= location;
    // cout << "Broken cell updated at row " << row << " ; " << broken[row] << endl;
}

inline bool isValid(int row, int state)
{
    /**
     * returns true if current state is valid
     * (does not put a horse on a broken cell)
     */
    return !(broken[row] & state);
}

inline bool checkFirstRowConflict(int row, int currentState, int firstRowState)
{
    if (firstRowState & firstRowConflictTable[currentState])
        return true;
    else
        return false;
}

inline bool checkSecondRowConflict(int row, int currentState, int secondRowState)
{
    if (row <= 2)
        return false;
    if (secondRowState & secondRowConflictTable[currentState])
        return true;
    else
        return false;
}

inline bool checkConflicts(int row, int state, int firstRowState, int secondRowState)
{
    /**
     * returns true if a conflict exist in the current state.
     * (horses cannot attack each other)
     */
    bool test = (checkFirstRowConflict(row, state, firstRowState) || checkSecondRowConflict(row, state, secondRowState));
    /* debugging purposes
    cout << "Curernt Row: " << row << "  ";
    cout << "Current State: " << state << "  ";
    cout << "First Row Conflict: " << firstRowConflictTable[state] << "  ";
    cout << "First Row: " << firstRowState << "  ";
    cout << "Second Row Conflict: " << secondRowConflictTable[state] << "  ";
    cout << "Second Row: " << secondRowState << "  ";
    cout << "Conflict?: " << test ? "True" : "False";
    cout << endl;
    */
    return test;
}

inline int count(int state)
{
    int count = 0;
    for (int i = 0, p = 1; i < MAX_STATES; ++i, p *= 2)
        count += state & p ? 1 : 0;

    return count;
}

int main()
{
    int m; // num of rows 1 \le m \le 200
    int n; // num of columns 1 \le n \le 6
    int k; // num of broken cells

    // input
    cin >> m >> n >> k;
    for (int i = 0; i < k; ++i)
    {
        int row;
        int column;
        cin >> row >> column;
        addBrokenCell(row, column);
    }

    // initialize
    int msk = 0;
    int totalStates = 1;
    for (int i = 0; i < n; ++i)
    {
        totalStates *= 2;
        msk = (msk << 1) + 1;
    }
    // cout << "Mask: " << msk << endl;
    initializeConflictTables(n, msk, totalStates);
    // cout << "Total States: " << totalStates << endl;
    for (int state = 0; state < totalStates; ++state)
    {
        if (isValid(1, state))
        {
            result[1][state][0] += 1;
        }
    }

    // dynamic program
    for (int row = 2; row <= m; ++row)
    {
        for (int currentState = 0; currentState < totalStates; ++currentState)
        {
            if (!isValid(row, currentState))
                continue;
            for (int firstRowState = 0; firstRowState < totalStates; ++firstRowState)
            {
                for (int secondRowState = 0; secondRowState < totalStates; ++secondRowState)
                {
                    if (checkConflicts(row, currentState, firstRowState, secondRowState))
                        continue;
                    result[row][currentState][firstRowState] += (result[row-1][firstRowState][secondRowState]);
                    result[row][currentState][firstRowState] %= CONST;
                }
            }
        }
    }
    // cout << "TST: " << result[2][2][0] << endl;
    
    int sum = 0;
    for (int i = 0; i < totalStates; ++i)
    {
        for (int j = 0; j < totalStates; ++j)
        {
            sum = (sum + result[m][i][j]) % CONST;
        }
    }
    cout << sum;

    return 0;
}
