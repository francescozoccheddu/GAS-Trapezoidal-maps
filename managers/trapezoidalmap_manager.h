#ifndef VORONOIMANAGER_H
#define VORONOIMANAGER_H

#include <QFrame>

#include <cg3/viewer/mainwindow.h>

#include <cg3/viewer/drawable_objects/drawable_bounding_box2.h>
#include <cg3/viewer/drawable_objects/drawable_point2.h>

#include <cg3/geometry/segment2.h>

#include "drawables/drawable_trapezoidalmap_dataset.h"

#include <gas/data/trapezoidal_map.hpp>
#include <gas/drawing/trapezoidal_map_drawer.hpp>
#include <cg3/viewer/interfaces/drawable_container.h>

namespace Ui {
    class TrapezoidalMapManager;
}

/**
 * @brief The manager which gives you all the functionalities needed by
 * the project.
 */
class TrapezoidalMapManager : public QFrame {
    Q_OBJECT

public:

	/* ----- Constructors/destructors ----- */

    explicit TrapezoidalMapManager(QWidget *parent = nullptr);
    ~TrapezoidalMapManager();


private:

	/* ----- Private fields ----- */

	//UI fields
	Ui::TrapezoidalMapManager *ui;
    cg3::viewer::MainWindow& mainWindow;    

	//It is const because, once defined and initialized, it will never change!
	const cg3::DrawableBoundingBox2 drawableBoundingBox;

	//Drawable dataset for the trapezoidal map. Each segment is consistent:
	//points in general position, no segment duplicates, non-intersecting segments
	DrawableTrapezoidalMapDataset drawableTrapezoidalMapDataset;

	//Variables to allow to select a segment clicking on the canvas
	cg3::DrawablePoint2 firstPointSelected;
	cg3::Color firstPointSelectedColor;
	unsigned int firstPointSelectedSize;
	bool isFirstPointSelected;



	//---------------------------------------------------------------------
	//Declare your attributes here

	using Scalar = double;

	// Trapezoidal map
	GAS::TrapezoidalMap<Scalar> m_trapezoidalMap;
	// Colorizers
	GAS::Drawing::TrapezoidColorizers::Constant<Scalar> m_trapezoidStrokeColorizer { { 0, 0, 0 } };
	GAS::Drawing::TrapezoidColorizers::Rainbow<Scalar> m_trapezoidNormalFillColorizer { 1.0f, 1.0f };
	GAS::Drawing::TrapezoidColorizers::Rainbow<Scalar> m_trapezoidSelectionFillColorizer { 1.0f, 1.0f, 0.25f };
	GAS::Drawing::TrapezoidColorizers::Rainbow<Scalar> m_trapezoidTextColorizer { 0.9f, 0.5f };
	GAS::Drawing::TrapezoidColorizers::Selector<Scalar> m_trapezoidSelectorFillColorizer { m_trapezoidNormalFillColorizer, m_trapezoidSelectionFillColorizer };
	// Renderers
	GAS::Drawing::TrapezoidRenderers::Stroke<Scalar> m_trapezoidStrokeRenderer { 2 };
	GAS::Drawing::TrapezoidRenderers::Fill<Scalar> m_trapezoidFillRenderer {};
	GAS::Drawing::TrapezoidRenderers::Text<Scalar> m_trapezoidTextRenderer { mainWindow.canvas };
	// Drawers
	GAS::Drawing::TrapezoidalMapDrawer<Scalar> m_trapezoidalMapStrokeDrawer { m_trapezoidalMap, m_trapezoidStrokeColorizer, m_trapezoidStrokeRenderer };
	GAS::Drawing::TrapezoidalMapDrawer<Scalar> m_trapezoidalMapFillDrawer { m_trapezoidalMap, m_trapezoidSelectorFillColorizer, m_trapezoidFillRenderer };
	GAS::Drawing::TrapezoidalMapDrawer<Scalar> m_trapezoidalMapTextDrawer { m_trapezoidalMap, m_trapezoidTextColorizer, m_trapezoidTextRenderer };
	// Drawable object
	cg3::DrawableContainer m_trapezoidalMapDrawableContainer {};

	//#####################################################################



	/* ----- Methods (YOU WILL HAVE TO EDIT THESE METHODS) ----- */

    void addSegmentToTrapezoidalMap(const cg3::Segment2d& segment);
    void queryTrapezoidalMap(const cg3::Point2d& point);
    void clearTrapezoidalMap();



	//---------------------------------------------------------------------
	//Declare your private methods here if you need some




	//#####################################################################



	/* ----- Private utility methods (DO NOT WRITE CODE IN THESE METHODS) ----- */

    void loadSegmentsTrapezoidalMapAndMeasureTime(const std::vector<cg3::Segment2d>& segments);
    void addSegmentToTrapezoidalMapAndMeasureTime(const cg3::Segment2d& segment);
    void queryTrapezoidalMapAndMeasureTime(const cg3::Point2d& point);
    std::vector<cg3::Segment2d> generateRandomNonIntersectingSegments(size_t n, double radius);

    void fitScene();
    void updateCanvas();


private slots:

    /* ----- UI slots ----- */

    void point2DClicked(cg3::Point2d p);

    void on_loadSegmentsButton_clicked();
    void on_randomSegmentsButton_clicked();
    void on_saveSegmentsButton_clicked();
    void on_addSegmentRadio_clicked();
    void on_queryRadio_clicked();
    void on_clearSegmentsButton_clicked();
    void on_resetSceneButton_clicked();
};

#endif // VORONOIMANAGER_H
