//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"

//
// TODO:Student Information
//
const char *studentName = "Er Zhuo";
const char *studentID   = "A59002689";
const char *email       = "erzhuo@ucsd.edu";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
//TODO: Add your own Branch Predictor data structures here
//
// data structures for gshare
uint32_t gshareMask;
uint32_t gshareHistory;
uint32_t* gshareCounts;

// data structures for tournament
uint32_t tournamentPCMask;
uint32_t tournamentLMask;
uint32_t* tournamentLHistories;
uint32_t* tournamentLCounts;

uint32_t tournamentGMask;
uint32_t tournamentGHistory;
uint32_t* tournamentGCounts;

uint32_t* tournamentChoices;

// data structures for custom
uint32_t customPCMask;
uint32_t customLMask;
uint32_t* customLHistories;
uint32_t* customLCounts;

uint32_t customGshareMask;
uint32_t customGshareHistory;
uint32_t* customGshareCounts;

uint32_t* customChoices;

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the gshare predictor
void
init_gshare()
{
    gshareMask = (uint32_t) (1 << ghistoryBits) - 1;

    gshareHistory = NOTTAKEN;

    gshareCounts = (uint32_t*) malloc((1 << ghistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << ghistoryBits); ++i) {
        gshareCounts[i] = WN;
    }
}

// Initialize the tournament predictor
void
init_tournament()
{
    tournamentPCMask = (uint32_t) (1 << pcIndexBits) - 1;

    tournamentLMask = (uint32_t) (1 << lhistoryBits) - 1;

    tournamentLHistories = (uint32_t*) malloc((1 << pcIndexBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << pcIndexBits); ++i) {
        tournamentLHistories[i] = NOTTAKEN;
    }

    tournamentLCounts = (uint32_t*) malloc((1 << lhistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << lhistoryBits); ++i) {
        tournamentLCounts[i] = WN;
    }

    tournamentGMask = (uint32_t) (1 << ghistoryBits) - 1;

    tournamentGHistory = NOTTAKEN;

    tournamentGCounts = (uint32_t*) malloc((1 << ghistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << ghistoryBits); ++i) {
        tournamentGCounts[i] = WN;
    }

    tournamentChoices = (uint32_t*) malloc((1 << ghistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << ghistoryBits); ++i) {
        tournamentChoices[i] = WN; // WN -> weakly global
    }
}

// Initialize the custom predictor
void
init_custom()
{
    ghistoryBits = 13;
    lhistoryBits = 11;
    pcIndexBits = 11;

    customPCMask = (uint32_t) (1 << pcIndexBits) - 1;

    customLMask= (uint32_t) (1 << lhistoryBits) - 1;

    customLHistories = (uint32_t*) malloc((1 << pcIndexBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << pcIndexBits); ++i) {
        customLHistories[i] = NOTTAKEN;
    }

    customLCounts = (uint32_t*) malloc((1 << lhistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << lhistoryBits); ++i) {
        customLCounts[i] = WN;
    }

    customGshareMask = (uint32_t) (1 << ghistoryBits) - 1;

    customGshareHistory = NOTTAKEN;

    customGshareCounts = (uint32_t*) malloc((1 << ghistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << ghistoryBits); ++i) {
        customGshareCounts[i] = WN;
    }

    customChoices = (uint32_t*) malloc((1 << ghistoryBits) * sizeof(uint32_t));
    for (int i = 0; i < (1 << ghistoryBits); ++i) {
        customChoices[i] = WN; // WN -> weakly global
    }
}

// Initialize the predictor
//
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
    switch (bpType) {
        case STATIC:
            return;
        case GSHARE:
            init_gshare();
            return;
        case TOURNAMENT:
            init_tournament();
            return;
        case CUSTOM:
            init_custom();
            return;
        default:
            break;
    }
}

// Make a prediction using gshare
uint8_t
make_prediction_gshare(uint32_t pc)
{
    uint32_t idx = (gshareMask & pc) ^ (gshareMask & gshareHistory);
    uint32_t cnt = gshareCounts[idx];

    return (cnt >= WT) ? TAKEN : NOTTAKEN;
}

// Make a prediction using tournament
uint8_t
make_prediction_tournament(uint32_t pc)
{
    uint32_t lIdx = tournamentLMask & tournamentLHistories[tournamentPCMask & pc];
    uint32_t lCnt = tournamentLCounts[lIdx];
    uint8_t lPred = (lCnt >= WT) ? TAKEN : NOTTAKEN;

    uint32_t gIdx = tournamentGMask & tournamentGHistory;
    uint32_t gCnt = tournamentGCounts[gIdx];
    uint8_t gPred = (gCnt >= WT) ? TAKEN : NOTTAKEN;

    uint32_t choiceCnt = tournamentChoices[gIdx];

    return (choiceCnt >= WT) ? lPred : gPred;
}

// Make a prediction using custom
uint8_t
make_prediction_custom(uint32_t pc)
{
    uint32_t lIdx = customLMask & customLHistories[customPCMask & pc];
    uint32_t lCnt = customLCounts[lIdx];
    uint8_t lPred = (lCnt >= WT) ? TAKEN : NOTTAKEN;

    uint32_t gIdx = (customGshareMask & pc) ^ (customGshareMask & customGshareHistory);
    uint32_t gCnt = customGshareCounts[gIdx];
    uint8_t gPred = (gCnt >= WT) ? TAKEN : NOTTAKEN;

    uint32_t choiceCnt = customChoices[gIdx];

    return (choiceCnt >= WT) ? lPred : gPred;
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
        return TAKEN;
    case GSHARE:
        return make_prediction_gshare(pc);
    case TOURNAMENT:
        return make_prediction_tournament(pc);
    case CUSTOM:
        return make_prediction_custom(pc);
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// train gshare
void
train_gshare(uint32_t pc, uint8_t outcome)
{
    uint32_t idx = (gshareMask & pc) ^ (gshareMask & gshareHistory);

    if (outcome == TAKEN && gshareCounts[idx] < ST) {
        ++gshareCounts[idx];
    }
    if (outcome == NOTTAKEN && gshareCounts[idx] > SN) {
        --gshareCounts[idx];
    }

    gshareHistory = (gshareHistory << 1) | outcome;
}

// train tournament
void
train_tournament(uint32_t pc, uint8_t outcome)
{
    uint32_t lIdx = tournamentLMask & tournamentLHistories[tournamentPCMask & pc];
    uint32_t lCnt = tournamentLCounts[lIdx];
    uint8_t lPred = (lCnt >= WT) ? TAKEN : NOTTAKEN;

    uint32_t gIdx = tournamentGMask & tournamentGHistory;
    uint32_t gCnt = tournamentGCounts[gIdx];
    uint8_t gPred = (gCnt >= WT) ? TAKEN : NOTTAKEN;

    if (lPred == outcome && gPred != outcome && tournamentChoices[gIdx] < ST) {
        ++tournamentChoices[gIdx];
    }
    if (lPred != outcome && gPred == outcome && tournamentChoices[gIdx] > SN) {
        --tournamentChoices[gIdx];
    }

    if (outcome == TAKEN) {
        if (tournamentLCounts[lIdx] < ST) {
            ++tournamentLCounts[lIdx];
        }
        if (tournamentGCounts[gIdx] < ST) {
            ++tournamentGCounts[gIdx];
        }
    }
    else {
        if (tournamentLCounts[lIdx] > SN) {
            --tournamentLCounts[lIdx];
        }
        if (tournamentGCounts[gIdx] > SN) {
            --tournamentGCounts[gIdx];
        }
    }

    tournamentLHistories[tournamentPCMask & pc] = (tournamentLHistories[tournamentPCMask & pc] << 1) | outcome;

    tournamentGHistory = (tournamentGHistory << 1) | outcome;
}

// train custom
void
train_custom(uint32_t pc, uint8_t outcome)
{
    uint32_t lIdx = customLMask & customLHistories[customPCMask & pc];
    uint32_t lCnt = customLCounts[lIdx];
    uint8_t lPred = (lCnt >= WT) ? TAKEN : NOTTAKEN;

    uint32_t gIdx = (customGshareMask & pc) ^ (customGshareMask & customGshareHistory);
    uint32_t gCnt = customGshareCounts[gIdx];
    uint8_t gPred = (gCnt >= WT) ? TAKEN : NOTTAKEN;

    if (lPred == outcome && gPred != outcome && customChoices[gIdx] < ST) {
        ++customChoices[gIdx];
    }
    if (lPred != outcome && gPred == outcome && customChoices[gIdx] > SN) {
        --customChoices[gIdx];
    }

    if (outcome == TAKEN) {
        if (customLCounts[lIdx] < ST) {
            ++customLCounts[lIdx];
        }
        if (customGshareCounts[gIdx] < ST) {
            ++customGshareCounts[gIdx];
        }
    }
    else {
        if (customLCounts[lIdx] > SN) {
            --customLCounts[lIdx];
        }
        if (customGshareCounts[gIdx] > SN) {
            --customGshareCounts[gIdx];
        }
    }

    customLHistories[customPCMask & pc] = (customLHistories[customPCMask & pc] << 1) | outcome;

    customGshareHistory = (customGshareHistory << 1) | outcome;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  //
  //TODO: Implement Predictor training
  //

  // Train predictor based on the bpType
    switch (bpType) {
        case STATIC:
            return;
        case GSHARE:
            train_gshare(pc, outcome);
            return;
        case TOURNAMENT:
            train_tournament(pc, outcome);
            return;
        case CUSTOM:
            train_custom(pc, outcome);
            return;
        default:
            break;
    }
}
