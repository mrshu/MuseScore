/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "testing/qtestsuite.h"
#include "testbase.h"

#include "libmscore/masterscore.h"
#include "libmscore/excerpt.h"
#include "libmscore/part.h"
#include "libmscore/undo.h"
#include "libmscore/measure.h"
#include "libmscore/measurerepeat.h"
#include "libmscore/chord.h"
#include "libmscore/note.h"
#include "libmscore/breath.h"
#include "libmscore/segment.h"
#include "libmscore/fingering.h"
#include "libmscore/image.h"
#include "libmscore/engravingitem.h"
#include "libmscore/staff.h"
#include "libmscore/stafftype.h"
#include "libmscore/sym.h"
#include "libmscore/chordline.h"
#include "libmscore/sym.h"

static const QString PARTS_DATA_DIR("parts_data/");

using namespace Ms;

//---------------------------------------------------------
//   TestParts
//---------------------------------------------------------

class TestParts : public QObject, public MTest
{
    Q_OBJECT

    void createParts(MasterScore* score);
    void testPartCreation(const QString& test);

    MasterScore* doAddBreath();
    MasterScore* doRemoveBreath();
    MasterScore* doAddFingering();
    MasterScore* doRemoveFingering();
    MasterScore* doAddSymbol();
    MasterScore* doRemoveSymbol();
    MasterScore* doAddChordline();
    MasterScore* doRemoveChordline();
    MasterScore* doAddMeasureRepeat();
    MasterScore* doRemoveMeasureRepeat();
//      MasterScore* doAddImage();
//      MasterScore* doRemoveImage();

private slots:
    void initTestCase();

    void createPart1();
    void createPart2();
    void voicesExcerpt();

    void createPartBreath();
    void addBreath();
    void undoAddBreath();
    void undoRedoAddBreath();
    void removeBreath();
    void undoRemoveBreath();
    void undoRedoRemoveBreath();

    void createPartFingering();
    void addFingering();
    void undoAddFingering();
    void undoRedoAddFingering();
    void removeFingering();
    void undoRemoveFingering();
    void undoRedoRemoveFingering();

    void createPartSymbol();
    void addSymbol();
    void undoAddSymbol();
    void undoRedoAddSymbol();
    void removeSymbol();
    void undoRemoveSymbol();
    void undoRedoRemoveSymbol();

    void createPartChordline();
    void addChordline();
    void undoAddChordline();
    void undoRedoAddChordline();
    void removeChordline();
    void undoRemoveChordline();
    void undoRedoRemoveChordline();

    void createPartMeasureRepeat();
    void addMeasureRepeat();
    void undoAddMeasureRepeat();
    void undoRedoAddMeasureRepeat();
    void removeMeasureRepeat();
    void undoRemoveMeasureRepeat();
    void undoRedoRemoveMeasureRepeat();

    void createPartStemless();

//      void createPartImage();
//      void addImage();
//      void undoAddImage();
//      void undoRedoAddImage();
//      void removeImage();
//      void undoRemoveImage();
//      void undoRedoRemoveImage();

    void appendMeasure();
    void insertMeasure();
//      void styleScore();
//      void styleScoreReload();
//      void stylePartDefault();
//      void styleScoreDefault();
//      void staffStyles();

    void measureProperties();

    // second part has system text on empty chordrest segment
    void createPart3()
    {
        testPartCreation("part-54346");
    }
};

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestParts::initTestCase()
{
    initMTest();
}

//---------------------------------------------------------
//   createParts
//---------------------------------------------------------

void TestParts::createParts(MasterScore* score)
{
    //
    // create first part
    //
    QList<Part*> parts;
    parts.append(score->parts().at(0));
    Score* nscore = new Score(score);

    Excerpt* ex = new Excerpt(score);
    ex->setPartScore(nscore);
    ex->setParts(parts);
    ex->setTitle(parts.front()->partName());
    Excerpt::createExcerpt(ex);
    score->excerpts().append(ex);
//      ex->setTitle(parts.front()->longName());
    QVERIFY(nscore);

//      nscore->setName(parts.front()->partName());

    //
    // create second part
    //
    parts.clear();
    parts.append(score->parts().at(1));
    nscore = new Score(score);

    ex = new Excerpt(score);
    ex->setPartScore(nscore);
    ex->setParts(parts);
    ex->setTitle(parts.front()->partName());
    Excerpt::createExcerpt(ex);
    score->excerpts().append(ex);
//      ex->setTitle(parts.front()->longName());
    QVERIFY(nscore);

//      nscore->setName(parts.front()->partName());

    score->setExcerptsChanged(true);
}

//---------------------------------------------------------
//   voicesExcerpt
//---------------------------------------------------------

void TestParts::voicesExcerpt()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "voices.mscx");

    //
    // create first part
    //
    QList<Part*> parts;
    QMultiMap<int, int> trackList;
    parts.append(score->parts().at(0));
    Score* nscore = new Score(score);

    trackList.insert(1, 0);
    trackList.insert(2, 1);
    trackList.insert(4, 4);

    Excerpt* ex = new Excerpt(score);
    ex->setPartScore(nscore);
    nscore->setExcerpt(ex);
    score->excerpts().append(ex);
    ex->setTitle(parts.front()->longName());
    ex->setParts(parts);
    ex->setTracks(trackList);
    Excerpt::createExcerpt(ex);
    QVERIFY(nscore);

//      nscore->setName(parts.front()->partName());

    //
    // create second part
    //
    parts.clear();
    parts.append(score->parts().at(1));
    nscore = new Score(score);

    trackList.clear();
    trackList.insert(11, 0);

    ex = new Excerpt(score);
    ex->setPartScore(nscore);
    nscore->setExcerpt(ex);
    score->excerpts().append(ex);
    ex->setTitle(parts.front()->longName());
    ex->setParts(parts);
    ex->setTracks(trackList);
    Excerpt::createExcerpt(ex);
    QVERIFY(nscore);

    //
    // create second part
    //
    parts.clear();
    parts.append(score->parts().at(1));
    nscore = new Score(score);

    trackList.clear();
    trackList.insert(8, 0);

    ex = new Excerpt(score);
    ex->setPartScore(nscore);
    nscore->setExcerpt(ex);
    score->excerpts().append(ex);
    ex->setTitle(parts.front()->longName());
    ex->setParts(parts);
    ex->setTracks(trackList);
    Excerpt::createExcerpt(ex);
    QVERIFY(nscore);

//      nscore->setName(parts.front()->partName());

    score->setExcerptsChanged(true);

    QVERIFY(saveCompareScore(score, "voices.mscx", PARTS_DATA_DIR + "voices-ref.mscx"));

    delete score;
}

//---------------------------------------------------------
//   testPartCreation
//---------------------------------------------------------

void TestParts::testPartCreation(const QString& test)
{
    MasterScore* score = readScore(PARTS_DATA_DIR + test + ".mscx");
    QVERIFY(score);
    QVERIFY(saveCompareScore(score, test + "-1.mscx", PARTS_DATA_DIR + test + ".mscx"));
    createParts(score);
    QVERIFY(saveCompareScore(score, test + "-parts.mscx", PARTS_DATA_DIR + test + "-parts.mscx"));
    delete score;
}

//---------------------------------------------------------
//   testAppendMeasure
//---------------------------------------------------------

void TestParts::appendMeasure()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-all.mscx");

    QVERIFY(score);
    createParts(score);

    score->startCmd();
    score->insertMeasure(ElementType::MEASURE, 0);
    score->endCmd();

    QVERIFY(saveCompareScore(score, "part-all-appendmeasures.mscx", PARTS_DATA_DIR + "part-all-appendmeasures.mscx"));

    score->undoRedo(true, 0);

    QVERIFY(saveCompareScore(score, "part-all-uappendmeasures.mscx", PARTS_DATA_DIR + "part-all-uappendmeasures.mscx"));
    delete score;
}

//---------------------------------------------------------
//   testInsertMeasure
//---------------------------------------------------------

void TestParts::insertMeasure()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-all.mscx");
    QVERIFY(score);
    createParts(score);

    score->startCmd();
    Measure* m = score->firstMeasure();
    score->insertMeasure(ElementType::MEASURE, m);
    score->endCmd();

    // QVERIFY(saveCompareScore(score, "part-all-insertmeasures.mscx", PARTS_DATA_DIR + "part-all-insertmeasures.mscx"));

    score->undoRedo(true, 0);

    QVERIFY(saveCompareScore(score, "part-all-uinsertmeasures.mscx", PARTS_DATA_DIR + "part-all-uinsertmeasures.mscx"));
    delete score;
}

#if 0
//---------------------------------------------------------
//   styleScore
//---------------------------------------------------------

void TestParts::styleScore()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "partStyle.mscx");
    QVERIFY(score);
    createParts(score);
    score->style().set(StyleIdx::clefLeftMargin, 4.0);
    QVERIFY(saveCompareScore(score, "partStyle-score-test.mscx", PARTS_DATA_DIR + "partStyle-score-ref.mscx"));
    delete score;
}

//---------------------------------------------------------
//   styleScoreReload
//---------------------------------------------------------

void TestParts::styleScoreReload()
{
    MasterScore* partScore = readScore(PARTS_DATA_DIR + "partStyle-score-reload.mscx");
    QVERIFY(saveCompareScore(partScore, "partStyle-score-reload-test.mscx", PARTS_DATA_DIR + "partStyle-score-reload-ref.mscx"));
    delete partScore;
}

#endif

//---------------------------------------------------------
//   stylePartDefault
//---------------------------------------------------------

#if 0
void TestParts::stylePartDefault()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "partStyle.mscx");
    QVERIFY(score);
    // TODO: set defaultStyleForParts
    MScore::_defaultStyleForParts = new MStyle();
    QFile f(PARTS_DATA_DIR + "style_test.mss");
    QVERIFY(f.open(QIODevice::ReadOnly));
    MStyle* s = new MStyle(*defaultStyle());
    QVERIFY(s->load(&f));
    MScore::_defaultStyleForParts = s;
    createParts(score);
    QVERIFY(saveCompareScore(score, "partStyle-part-default-test.mscx", PARTS_DATA_DIR + "partStyle-part-default-ref.mscx"));
}

//---------------------------------------------------------
//   styleScoreDefault
//---------------------------------------------------------

void TestParts::styleScoreDefault()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "partStyle.mscx");
    QVERIFY(score);
    // TODO: set defaultStyle
    createParts(score);
    QVERIFY(saveCompareScore(score, "partStyle-score-default-test.mscx", PARTS_DATA_DIR + "partStyle-score-default-ref.mscx"));
}

#endif

//---------------------------------------------------------
//   test part creation
//---------------------------------------------------------

void TestParts::createPart1()
{
    testPartCreation("part-empty");
}

void TestParts::createPart2()
{
    testPartCreation("part-all");
}

void TestParts::createPartBreath()
{
    testPartCreation("part-breath");
}

void TestParts::createPartFingering()
{
    testPartCreation("part-fingering");
}

void TestParts::createPartSymbol()
{
    testPartCreation("part-symbol");
}

void TestParts::createPartChordline()
{
    testPartCreation("part-chordline");
}

void TestParts::createPartMeasureRepeat()
{
    testPartCreation("part-measure-repeat");
}

void TestParts::createPartStemless()
{
    testPartCreation("part-stemless");
}

#if 0
void TestParts::createPartImage()
{
    testPartCreation("part-image");
}

#endif
//---------------------------------------------------------
//    doAddBreath
//---------------------------------------------------------

MasterScore* TestParts::doAddBreath()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-empty-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->tick2segment(Fraction(1, 4));
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EditData dd(0);
    Breath* b = new Breath(score);
    b->setSymId(SymId::breathMarkComma);
    dd.dropElement = b;

    score->startCmd();
    note->drop(dd);
    score->endCmd();          // does layout

    return score;
}

//---------------------------------------------------------
//   addBreath
//---------------------------------------------------------

void TestParts::addBreath()
{
    MasterScore* score = doAddBreath();
    QVERIFY(saveCompareScore(score, "part-breath-add.mscx", PARTS_DATA_DIR + "part-breath-add.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoAddBreath
//---------------------------------------------------------

void TestParts::undoAddBreath()
{
    MasterScore* score = doAddBreath();

    score->undoRedo(true, 0);

    QVERIFY(saveCompareScore(score, "part-breath-uadd.mscx", PARTS_DATA_DIR + "part-breath-uadd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoAddBreath
//---------------------------------------------------------

void TestParts::undoRedoAddBreath()
{
    MasterScore* score = doAddBreath();

    score->undoRedo(true, 0);
    score->undoRedo(false, 0);

    QVERIFY(saveCompareScore(score, "part-breath-uradd.mscx", PARTS_DATA_DIR + "part-breath-uradd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doRemoveBreath
//---------------------------------------------------------

MasterScore* TestParts::doRemoveBreath()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-breath-add.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->first()->next(SegmentType::Breath);
    Breath* b    = toBreath(s->element(0));

    score->select(b);
    score->startCmd();
    score->cmdDeleteSelection();
    score->setLayoutAll();
    score->endCmd();
    return score;
}

//---------------------------------------------------------
//   removeBreath
//---------------------------------------------------------

void TestParts::removeBreath()
{
    MasterScore* score = doRemoveBreath();
    QVERIFY(saveCompareScore(score, "part-breath-del.mscx", PARTS_DATA_DIR + "part-breath-del.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRemoveBreath
//---------------------------------------------------------

void TestParts::undoRemoveBreath()
{
    MasterScore* score = doRemoveBreath();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-breath-udel.mscx", PARTS_DATA_DIR + "part-breath-udel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoRemoveBreath
//---------------------------------------------------------

void TestParts::undoRedoRemoveBreath()
{
    MasterScore* score = doRemoveBreath();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);

    QVERIFY(saveCompareScore(score, "part-breath-urdel.mscx", PARTS_DATA_DIR + "part-breath-urdel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doAddFingering
//---------------------------------------------------------

MasterScore* TestParts::doAddFingering()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-empty-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->tick2segment(Fraction(1, 4));
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EditData dd(0);
    Fingering* b = new Fingering(score);
    b->setXmlText("3");
    dd.dropElement = b;

    score->startCmd();
    note->drop(dd);
    score->endCmd();          // does layout
    return score;
}

//---------------------------------------------------------
//   addFingering
//---------------------------------------------------------

void TestParts::addFingering()
{
    MasterScore* score = doAddFingering();
    QVERIFY(saveCompareScore(score, "part-fingering-add.mscx", PARTS_DATA_DIR + "part-fingering-add.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoAddFingering
//---------------------------------------------------------

void TestParts::undoAddFingering()
{
    MasterScore* score = doAddFingering();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-fingering-uadd.mscx", PARTS_DATA_DIR + "part-fingering-uadd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoAddFingering
//---------------------------------------------------------

void TestParts::undoRedoAddFingering()
{
    MasterScore* score = doAddFingering();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part-fingering-uradd.mscx", PARTS_DATA_DIR + "part-fingering-uradd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doRemoveFingering
//---------------------------------------------------------

MasterScore* TestParts::doRemoveFingering()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-fingering-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->first()->next(SegmentType::ChordRest);
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EngravingItem* fingering = 0;
    for (EngravingItem* e : note->el()) {
        if (e->type() == ElementType::FINGERING) {
            fingering = e;
            break;
        }
    }
    score->select(fingering);

    score->startCmd();
    score->cmdDeleteSelection();
    score->setLayoutAll();
    score->endCmd();
    return score;
}

//---------------------------------------------------------
//   removeFingering
//---------------------------------------------------------

void TestParts::removeFingering()
{
    MasterScore* score = doRemoveFingering();
    QVERIFY(saveCompareScore(score, "part-fingering-del.mscx", PARTS_DATA_DIR + "part-fingering-del.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRemoveFingering
//---------------------------------------------------------

void TestParts::undoRemoveFingering()
{
    MasterScore* score = doRemoveFingering();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-fingering-udel.mscx", PARTS_DATA_DIR + "part-fingering-udel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoRemoveFingering
//---------------------------------------------------------

void TestParts::undoRedoRemoveFingering()
{
    MasterScore* score = doRemoveFingering();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part-fingering-urdel.mscx", PARTS_DATA_DIR + "part-fingering-urdel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doAddSymbol
//---------------------------------------------------------

MasterScore* TestParts::doAddSymbol()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-empty-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->tick2segment(Fraction(1, 4));
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EditData dd(0);
    Symbol* b  = new Symbol(score);
    b->setSym(SymId::gClef);
    dd.dropElement = b;

    score->startCmd();
    note->drop(dd);
    score->endCmd();          // does layout
    return score;
}

//---------------------------------------------------------
//   addSymbol
//---------------------------------------------------------

void TestParts::addSymbol()
{
    MasterScore* score = doAddSymbol();
    QVERIFY(saveCompareScore(score, "part-symbol-add.mscx", PARTS_DATA_DIR + "part-symbol-add.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoAddSymbol
//---------------------------------------------------------

void TestParts::undoAddSymbol()
{
    MasterScore* score = doAddSymbol();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-symbol-uadd.mscx", PARTS_DATA_DIR + "part-symbol-uadd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoAddSymbol
//---------------------------------------------------------

void TestParts::undoRedoAddSymbol()
{
    MasterScore* score = doAddSymbol();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part-symbol-uradd.mscx", PARTS_DATA_DIR + "part-symbol-uradd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doRemoveSymbol
//---------------------------------------------------------

MasterScore* TestParts::doRemoveSymbol()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-symbol-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->first()->next(SegmentType::ChordRest);
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EngravingItem* se = 0;
    for (EngravingItem* e : note->el()) {
        if (e->type() == ElementType::SYMBOL) {
            se = e;
            break;
        }
    }
    score->select(se);

    score->startCmd();
    score->cmdDeleteSelection();
    score->setLayoutAll();
    score->endCmd();
    return score;
}

//---------------------------------------------------------
//   removeSymbol
//---------------------------------------------------------

void TestParts::removeSymbol()
{
    MasterScore* score = doRemoveSymbol();
    QVERIFY(saveCompareScore(score, "part-symbol-del.mscx", PARTS_DATA_DIR + "part-symbol-del.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRemoveSymbol
//---------------------------------------------------------

void TestParts::undoRemoveSymbol()
{
    MasterScore* score = doRemoveSymbol();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-symbol-udel.mscx", PARTS_DATA_DIR + "part-symbol-udel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoRemoveSymbol
//---------------------------------------------------------

void TestParts::undoRedoRemoveSymbol()
{
    MasterScore* score = doRemoveSymbol();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part-symbol-urdel.mscx", PARTS_DATA_DIR + "part-symbol-urdel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doAddChordline
//---------------------------------------------------------

MasterScore* TestParts::doAddChordline()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-empty-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->tick2segment(Fraction(1, 4));
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EditData dd(0);
    ChordLine* b  = new ChordLine(score);
    b->setChordLineType(ChordLineType::FALL);
    dd.dropElement = b;

    score->startCmd();
    note->drop(dd);
    score->endCmd();          // does layout
    return score;
}

//---------------------------------------------------------
//   addChordline
//---------------------------------------------------------

void TestParts::addChordline()
{
    MasterScore* score = doAddChordline();
    QVERIFY(saveCompareScore(score, "part-chordline-add.mscx", PARTS_DATA_DIR + "part-chordline-add.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoAddChordline
//---------------------------------------------------------

void TestParts::undoAddChordline()
{
    MasterScore* score = doAddChordline();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-chordline-uadd.mscx", PARTS_DATA_DIR + "part-chordline-uadd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoAddChordline
//---------------------------------------------------------

void TestParts::undoRedoAddChordline()
{
    MasterScore* score = doAddChordline();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part-chordline-uradd.mscx", PARTS_DATA_DIR + "part-chordline-uradd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doRemoveChordline
//---------------------------------------------------------

MasterScore* TestParts::doRemoveChordline()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-chordline-parts.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->first()->next(SegmentType::ChordRest);
    Ms::Chord* chord = toChord(s->element(0));

    EngravingItem* se = 0;
    for (EngravingItem* e : chord->el()) {
        if (e->type() == ElementType::CHORDLINE) {
            se = e;
            break;
        }
    }
    score->select(se);

    score->startCmd();
    score->cmdDeleteSelection();
    score->setLayoutAll();
    score->endCmd();
    return score;
}

//---------------------------------------------------------
//   removeChordline
//---------------------------------------------------------

void TestParts::removeChordline()
{
    MasterScore* score = doRemoveChordline();
    QVERIFY(saveCompareScore(score, "part-chordline-del.mscx", PARTS_DATA_DIR + "part-chordline-del.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRemoveChordline
//---------------------------------------------------------

void TestParts::undoRemoveChordline()
{
    MasterScore* score = doRemoveChordline();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-chordline-udel.mscx", PARTS_DATA_DIR + "part-chordline-udel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoRemoveChordline
//---------------------------------------------------------

void TestParts::undoRedoRemoveChordline()
{
    MasterScore* score = doRemoveChordline();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part-chordline-urdel.mscx", PARTS_DATA_DIR + "part-chordline-urdel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doAddMeasureRepeat
//---------------------------------------------------------

MasterScore* TestParts::doAddMeasureRepeat()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-empty-parts.mscx");
    Measure* m = score->firstMeasure()->nextMeasure();

    score->startCmd();
    score->cmdAddMeasureRepeat(m, 4, 0); // test with 4-measure repeat in first staff
    score->setLayoutAll();
    score->endCmd();

    return score;
}

//---------------------------------------------------------
//   addMeasureRepeat
//---------------------------------------------------------

void TestParts::addMeasureRepeat()
{
    MasterScore* score = doAddMeasureRepeat();
    QVERIFY(saveCompareScore(score, "part-measure-repeat-add.mscx", PARTS_DATA_DIR + "part-measure-repeat-add.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoAddMeasureRepeat
//---------------------------------------------------------

void TestParts::undoAddMeasureRepeat()
{
    MasterScore* score = doAddMeasureRepeat();

    score->undoRedo(true, 0);

    QVERIFY(saveCompareScore(score, "part-measure-repeat-uadd.mscx", PARTS_DATA_DIR + "part-measure-repeat-uadd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoAddMeasureRepeat
//---------------------------------------------------------

void TestParts::undoRedoAddMeasureRepeat()
{
    MasterScore* score = doAddMeasureRepeat();

    score->undoRedo(true, 0);
    score->undoRedo(false, 0);

    QVERIFY(saveCompareScore(score, "part-measure-repeat-uradd.mscx", PARTS_DATA_DIR + "part-measure-repeat-uradd.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doRemoveMeasureRepeat
//---------------------------------------------------------

MasterScore* TestParts::doRemoveMeasureRepeat()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part-measure-repeat-parts.mscx");

    Measure* m = score->firstMeasure()->nextMeasure()->nextMeasure();
    MeasureRepeat* mr = m->measureRepeatElement(0);
    score->select(mr);

    score->startCmd();
    score->cmdDeleteSelection();
    score->setLayoutAll();
    score->endCmd();
    return score;
}

//---------------------------------------------------------
//   removeMeasureRepeat
//---------------------------------------------------------

void TestParts::removeMeasureRepeat()
{
    MasterScore* score = doRemoveMeasureRepeat();
    QVERIFY(saveCompareScore(score, "part-measure-repeat-del.mscx", PARTS_DATA_DIR + "part-measure-repeat-del.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRemoveMeasureRepeat
//---------------------------------------------------------

void TestParts::undoRemoveMeasureRepeat()
{
    MasterScore* score = doRemoveMeasureRepeat();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part-measure-repeat-udel.mscx", PARTS_DATA_DIR + "part-measure-repeat-udel.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoRemoveMeasureRepeat
//---------------------------------------------------------

void TestParts::undoRedoRemoveMeasureRepeat()
{
    MasterScore* score = doRemoveMeasureRepeat();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);

    QVERIFY(saveCompareScore(score, "part-measure-repeat-urdel.mscx", PARTS_DATA_DIR + "part-measure-repeat-urdel.mscx"));
    delete score;
}

#if 0
//---------------------------------------------------------
//   doAddImage
//---------------------------------------------------------

MasterScore* TestParts::doAddImage()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part1-2o.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->tick2segment(MScore::division);
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EditData dd(0);
    RasterImage* b = new RasterImage(score);
    b->load(PARTS_DATA_DIR + "schnee.png");
    dd.dropElement = b;

    score->startCmd();
    note->drop(dd);
    score->endCmd();          // does layout
    return score;
}

//---------------------------------------------------------
//   addImage
//---------------------------------------------------------

void TestParts::addImage()
{
    MasterScore* score = doAddImage();
    QVERIFY(saveCompareScore(score, "part25.mscx", PARTS_DATA_DIR + "part25o.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoAddImage
//---------------------------------------------------------

void TestParts::undoAddImage()
{
    MasterScore* score = doAddImage();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part26.mscx", PARTS_DATA_DIR + "part26o.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoAddImage
//---------------------------------------------------------

void TestParts::undoRedoAddImage()
{
    MasterScore* score = doAddImage();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part27.mscx", PARTS_DATA_DIR + "part27o.mscx"));
    delete score;
}

//---------------------------------------------------------
//   doRemoveImage
//---------------------------------------------------------

MasterScore* TestParts::doRemoveImage()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part12o.mscx");

    Measure* m   = score->firstMeasure();
    Segment* s   = m->first()->next(SegChordRest);
    Ms::Chord* chord = toChord(s->element(0));
    Note* note   = chord->upNote();
    EngravingItem* fingering = 0;
    for (EngravingItem* e : note->el()) {
        if (e->type() == IMAGE) {
            fingering = e;
            break;
        }
    }
    score->select(fingering);

    score->startCmd();
    score->cmdDeleteSelection();
    score->setLayoutAll();
    score->endCmd();
    return score;
}

//---------------------------------------------------------
//   removeImage
//---------------------------------------------------------

void TestParts::removeImage()
{
    MasterScore* score = doRemoveImage();
    QVERIFY(saveCompareScore(score, "part28.mscx", PARTS_DATA_DIR + "part28o.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRemoveImage
//---------------------------------------------------------

void TestParts::undoRemoveImage()
{
    MasterScore* score = doRemoveImage();
    score->undoRedo(true, 0);
    QVERIFY(saveCompareScore(score, "part29.mscx", PARTS_DATA_DIR + "part29o.mscx"));
    delete score;
}

//---------------------------------------------------------
//   undoRedoRemoveImage
//---------------------------------------------------------

void TestParts::undoRedoRemoveImage()
{
    MasterScore* score = doRemoveImage();
    score->undoRedo(true, 0);
    score->undoRedo(false, 0);
    QVERIFY(saveCompareScore(score, "part30.mscx", PARTS_DATA_DIR + "part30o.mscx"));
    delete score;
}

#endif

//---------------------------------------------------------
//   undoRedoRemoveImage
//---------------------------------------------------------

#if 0
void TestParts::staffStyles()
{
    MasterScore* score = readScore(PARTS_DATA_DIR + "part1.mscx");
    QVERIFY(score);
//      int numOfStaffTypes = score->staffTypes().count();
    createParts(score);
    // check the number of staff styles did not change
//      QVERIFY(numOfStaffTypes == score->staffTypes().count());
    // modify a staff type
    int numOfLines = score->staffType(0)->lines() - 1;
    StaffType* newStaffType = score->staffType(0)->clone();
    newStaffType->setLines(numOfLines);
    score->addStaffType(0, newStaffType);
    // check the number of staff lines is correctly updated in root score and in parts
    QVERIFY(score->staff(0)->lines() == numOfLines);
    Excerpt* part = score->excerpts().at(0);
    QVERIFY(part->score()->staff(0)->lines() == numOfLines);
    part = score->excerpts().at(1);
    QVERIFY(part->score()->staff(0)->lines() == numOfLines);
    delete score;
}

#endif

//---------------------------------------------------------
//   measureProperties
//---------------------------------------------------------

void TestParts::measureProperties()
{
}

QTEST_MAIN(TestParts)

#include "tst_parts.moc"
