#pragma once

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

class QGraphicsPDFViewer;

/**
 * @brief PDF Viewer Toolbar for QGraphicsPDFViewer
 *
 * Provides navigation, zoom, rotation, and view mode controls similar to the
 * toolbar in the original PDFViewer class, designed specifically for use with
 * QGraphicsPDFViewer.
 */
class QGraphicsPDFViewerToolbar : public QWidget {
    Q_OBJECT

public:
    explicit QGraphicsPDFViewerToolbar(QWidget* parent = nullptr);
    ~QGraphicsPDFViewerToolbar() = default;

    /**
     * @brief Sets the associated QGraphicsPDFViewer
     * @param viewer The QGraphicsPDFViewer instance
     */
    void setViewer(QGraphicsPDFViewer* viewer);

    /**
     * @brief Updates the toolbar controls based on current document state
     * @param hasDocument Whether a document is currently loaded
     * @param currentPage Current page number (0-based)
     * @param pageCount Total number of pages
     * @param zoomFactor Current zoom factor (1.0 = 100%)
     * @param rotation Current rotation angle (0-360 degrees)
     */
    void updateControls(bool hasDocument, int currentPage, int pageCount,
                        double zoomFactor, int rotation);

    /**
     * @brief Sets the view mode combobox to the specified mode
     * @param mode View mode index
     */
    void setViewMode(int mode);

signals:
    // Navigation signals
    void firstPageRequested();
    void previousPageRequested();
    void nextPageRequested();
    void lastPageRequested();
    void pageNumberChanged(int pageNumber);

    // Zoom signals
    void zoomInRequested();
    void zoomOutRequested();
    void zoomToFitRequested();
    void zoomToWidthRequested();
    void zoomToHeightRequested();
    void zoomPercentageChanged(int percentage);

    // Rotation signals
    void rotateLeftRequested();
    void rotateRightRequested();

    // View mode signals
    void viewModeChanged(int mode);

private slots:
    void onPageNumberSpinBoxChanged(int value);
    void onZoomSliderChanged(int value);
    void onZoomPercentageSpinBoxChanged(int value);
    void onViewModeComboBoxChanged(int index);

private:
    void setupUI();
    void setupConnections();

    QHBoxLayout* m_mainLayout;

    // Page navigation controls
    QPushButton* m_firstPageBtn;
    QPushButton* m_prevPageBtn;
    QSpinBox* m_pageNumberSpinBox;
    QLabel* m_pageCountLabel;
    QPushButton* m_nextPageBtn;
    QPushButton* m_lastPageBtn;

    // Zoom controls
    QPushButton* m_zoomInBtn;
    QPushButton* m_zoomOutBtn;
    QSlider* m_zoomSlider;
    QSpinBox* m_zoomPercentageSpinBox;
    QPushButton* m_fitWidthBtn;
    QPushButton* m_fitHeightBtn;
    QPushButton* m_fitPageBtn;

    // Rotation controls
    QPushButton* m_rotateLeftBtn;
    QPushButton* m_rotateRightBtn;

    // View mode control
    QComboBox* m_viewModeComboBox;

    QGraphicsPDFViewer* m_viewer;
    bool m_updatingControls;
};