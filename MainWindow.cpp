#include "MainWindow.h"

const char* generate_method_str[] = { "DeepFirstSearch", "Prim", "Kruskal" };


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* fileMenu = menuBar->addMenu("File");
	QMenu* GenerationMenu = menuBar->addMenu("Generation Algorithm");
	//
	QMenu* SolveMenu = menuBar->addMenu("Solve Algorithm");
	QAction* DeepFirstSearchAction = GenerationMenu->addAction("DeepFirstSearch");
	QAction* PrimAction = GenerationMenu->addAction("Prim");
	QAction* KruskalAction = GenerationMenu->addAction("Kruskal");

	connect(DeepFirstSearchAction, &QAction::triggered, this, 
		[this] {
			drawThread->method = Generate_method::DeepFirstSearch; 
			emit methodChanged(Generate_method::DeepFirstSearch);
		}
	);

	connect(PrimAction, &QAction::triggered, this, 
		[this] {
		drawThread->method = Generate_method::DeepFirstSearch;
		emit methodChanged(Generate_method::Prim);
		}
	);

	connect(KruskalAction, &QAction::triggered, this, 
		[this] {
			drawThread->method = Generate_method::Kruskal;
			emit methodChanged(Generate_method::Kruskal);
		}
	);

	label_generate_method = new QLabel("Generation Method: \n" + QString(generate_method_str[0]), this);
	label_generate_method->setGeometry(20, 330, 150, 50);
	
	connect(this, &MainWindow::methodChanged, this, &MainWindow::onMethodChanged);

    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    setMenuBar(menuBar);

    QToolBar* toolBar = new QToolBar(this);
    toolBar->addAction(exitAction);
    addToolBar(toolBar);


    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

	mazeWidget = new MazeWidget(this);

	

	showInfoWidget = new ShowInfoWidget(this);
	showInfoWidget->setGeometry(10, 60, 160, 140);

	

	row_cnt = new QSpinBox(this);
	row_cnt->setGeometry(100, 210, 50, 20);
	row_cnt->setMinimum(10);
	col_cnt = new QSpinBox(this);
	col_cnt->setGeometry(100, 240, 50, 20);
	col_cnt->setMinimum(10);

	drawThread = new DrawThread(mazeWidget, row_cnt->value(), col_cnt->value(), Generate_method::DeepFirstSearch);

	QFont monospaceFont("Courier New", 10, true);
	label_row = new QLabel("Row", this);
	label_row->setGeometry(20, 210, 50, 20);
	label_row->setFont(monospaceFont);
	label_col = new QLabel("Column", this);
	label_col->setGeometry(20, 240, 50, 20);
	label_col->setFont(monospaceFont);

	button_generate = new QPushButton("Generate", this);
	button_generate->setGeometry(20, 270, 120, 30);
	button_generate->setFont(monospaceFont);
	connect(button_generate, &QPushButton::clicked, this, &MainWindow::generateButtonClicked);
	connect(drawThread, &DrawThread::imageReady, this, &MainWindow::onImageReady);

	button_findpath = new QPushButton("Find Path", this);
	button_findpath->setGeometry(20, 400, 120, 30);
	button_findpath->setFont(monospaceFont);
	connect(button_findpath, &QPushButton::clicked, this, &MainWindow::findPathButtonClicked);

	combobox_findpath_method = new QComboBox(this);
	combobox_findpath_method->setGeometry(20, 440, 120, 20);
	combobox_findpath_method->addItem("DeepFirstSearch");
	combobox_findpath_method->addItem("BreadthFirstSearch");
	combobox_findpath_method->addItem("Dijkstra");
	combobox_findpath_method->addItem("AStar");
	combobox_findpath_method->setFont(monospaceFont);
	connect(combobox_findpath_method, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboboxFindPathMethodChanged);

	connect(showInfoWidget->checkbox_show_wall, &QCheckBox::stateChanged, this, [&]() {
		drawThread->showWall = !drawThread->showWall;
		drawThread->BaseImage = drawThread->paintMaze();
		emit drawThread->imageReady(drawThread->paintPath());
		
		});
	connect(showInfoWidget->checkbox_show_path, &QCheckBox::stateChanged, this, [&]() {
		drawThread->showPath = !drawThread->showPath;
		drawThread->BaseImage = drawThread->paintMaze();
		emit drawThread->imageReady(drawThread->paintPath());
		});
	connect(showInfoWidget->checkbox_show_solution, &QCheckBox::stateChanged, this, [&]() {
		drawThread->showSolution = !drawThread->showSolution;
		drawThread->BaseImage = drawThread->paintMaze();
		emit drawThread->imageReady(drawThread->paintPath());
		});

	//button_generate->setStyleSheet("background-color: lightgrey");
    /*QDockWidget* dockWidget = new QDockWidget("Dockable", this);
    QLabel* dockLabel = new QLabel("This is a dockable widget", dockWidget);
    dockWidget->setWidget(dockLabel);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);*/
}

MainWindow::~MainWindow()
{}

void MainWindow::generateButtonClicked()
{
	if (drawThread->isDrawing)
	{
		return;
	}
	drawThread->isGenerated = false;
	drawThread->row = row_cnt->value();
	drawThread->column = col_cnt->value();
	drawThread->start();
}


MazeWidget::MazeWidget(QWidget* parent) : QWidget(parent)
{
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	maze = new AdjacencyList(10, 10);

	setGeometry(170, 60, 600, 500);
	repaint();
}


void MazeWidget::paintEvent(QPaintEvent* event)
{
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

	if (image != nullptr)
	{
		painter.drawImage(0, 0, *image);
	}
    
}

//void MazeWidget::paintMaze(QPainter& painter)
//{
//	painter.setPen(QPen(Qt::black, 1));
//	QRect r = rect().adjusted(1, 1, -1, -1);
//
//	painter.drawRect(r);
//	int row = maze->getRow();
//	int column = maze->getColumn();
//
//	int cell_width = r.width() / column;
//	int cell_height = r.height() / row;
//
//	int less_pixel_x = r.width() - cell_width * column;
//	int less_pixel_y = r.height() - cell_height * row;
//
//	int start_x = 1;
//	int start_y = 1;
//
//	int temp_x = 0;
//	int temp_y = 0;
//
//	for (int i = 0; i < row; i++)
//	{
//		if (temp_y > row) {
//			start_y += 1;
//			temp_y -= row;
//		}
//
//		temp_x = 0;
//		start_x = 0;
//		for (int j = 0; j < column; j++)
//		{
//			if (temp_x > column) {
//				start_x += 1;
//				temp_x -= column;
//			}
//
//			int index = i * column + j;
//			if (!maze->isNeighbor(index, index + 1))
//			{
//				painter.drawLine(start_x + cell_width, start_y, start_x + cell_width, start_y + cell_height);
//			}
//			if (!maze->isNeighbor(index, index + column))
//			{
//				painter.drawLine(start_x, start_y + cell_height, start_x + cell_width, start_y + cell_height);
//			}
//
//			temp_x += less_pixel_x;
//			start_x += cell_width;
//		}
//
//		start_y += cell_height;
//		temp_y += less_pixel_y;
//	}
//}
//
//
//
//void MazeWidget::generateMaze(int row, int column)
//{
//	maze = new AdjacencyList(row, column);
//
//	switch (method)
//	{
//	case Generate_method::DeepFirstSearch:
//		generateMazeByDeepFirstSearch(row, column);
//		break;
//
//	case Generate_method::Prim:
//
//		break;
//
//	case Generate_method::Kruskal:
//
//		break;
//	}
//}
//
//void MazeWidget::generateMazeByDeepFirstSearch(int row, int column)
//{
//	std::vector<int> visited(row * column, 0);
//	std::stack<int> stack;
//	stack.push(0);
//	visited[0] = 1;
//
//	while (!stack.empty())
//	{
//		int current = stack.top();
//
//		std::vector<int> surround = *maze->getSurround(current);
//		std::random_shuffle(surround.begin(), surround.end());
//
//		int i = 0;
//		for (; i < surround.size(); i++)
//		{
//			int temp = surround[i];
//			if (visited[temp] == 0)
//			{
//				maze->connect(current, temp);
//				visited[temp] = 1;
//				stack.push(temp);
//				break;
//			}
//		}
//
//		if (i == surround.size())
//		{
//			stack.pop();
//		}
//
//		repaint();
//		//update();
//	}
//}

ShowInfoWidget::ShowInfoWidget(QWidget* parent) : QWidget(parent)
{
	

	int label_x = 50;
	int checkbox_y_offset = 30;
	int label_width = 50;
	int label_height = 25;
	int checkbox_cnt = 0;
    QFont monospaceFont("Courier New", 10, true);

    label_show = new QLabel("Show", this);
    label_show->setGeometry(20, 0, 50, 20);
	label_show->setFont(monospaceFont);
    label_show->show();

	/*auto setLabel = [&](QLabel* label, const QString& text)
		{
			label = new QLabel(text, this);
			label->setGeometry(label_x, 20 + label_y_offset * label_cnt, label_width, label_height);
			label_cnt++;
			label->setFont(monospaceFont);
			label->adjustSize();
			label->show();
		};

	setLabel(label_wall, "Wall");
	setLabel(label_path, "Path");
	setLabel(label_solution, "Solution");
	setLabel(label_accessed, "Accessed");*/

	int checkbox_width = 20;
	int checkbox_height = 20;

    auto setCheckBox = [&](QCheckBox* &checkbox, const QString& text) {
		checkbox = new QCheckBox(text,this);
		checkbox->setGeometry(20, 20 + checkbox_y_offset * checkbox_cnt, checkbox_width, checkbox_height);
		checkbox_cnt++;
		//checkbox->setText(text);
		checkbox->setFont(monospaceFont);
		checkbox->adjustSize();
		checkbox->show();
		};

	setCheckBox(checkbox_show_wall, "Wall");
	checkbox_show_wall->setChecked(true);
	setCheckBox(checkbox_show_path, "Path");
	checkbox_show_path->setChecked(false);
	setCheckBox(checkbox_show_solution, "Solution");
	checkbox_show_solution->setChecked(true);
	setCheckBox(checkbox_show_accessed, "Accessed");

}

void ShowInfoWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.setPen(QPen(Qt::gray, 1));
    QRect r = rect().adjusted(10, 10, -10, -10);

	painter.drawLine(r.topLeft(), r.topLeft() + QPoint(10,0));
	painter.drawLine(r.topRight() - QPoint(95,0), r.topRight());
    painter.drawLine(r.bottomLeft(), r.bottomRight());
    painter.drawLine(r.topLeft(), r.bottomLeft());  
    painter.drawLine(r.topRight(), r.bottomRight()); 

}



QImage DrawThread::paintMaze()
{



	QImage image(600, 500, QImage::Format_RGB32);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	painter.setPen(QPen(Qt::black, 1));
	QRect r = image.rect().adjusted(1, 1, -1, -1);

	if (showWall == false) {
		return image;
	}

	int row = maze->getRow();
	int column = maze->getColumn();

	int cell_width = r.width() / column;
	int cell_height = r.height() / row;

	int less_pixel_x = r.width() - cell_width * column;
	int less_pixel_y = r.height() - cell_height * row;

	int start_x = 1;
	int start_y = 1;

	int temp_x = 0;
	int temp_y = 0;

	for (int i = 0; i < row; i++)
	{
		if (temp_y > row) {
			start_y += 1;
			temp_y -= row;
		}

		temp_x = 0;
		start_x = 0;
		for (int j = 0; j < column; j++)
		{
			if (temp_x > column) {
				start_x += 1;
				temp_x -= column;
			}

			int index = i * column + j;
			if (!maze->isNeighbor(index, index + 1))
			{
				painter.drawLine(start_x + cell_width, start_y, start_x + cell_width, start_y + cell_height);
			}
			if (!maze->isNeighbor(index, index + column))
			{
				painter.drawLine(start_x, start_y + cell_height, start_x + cell_width, start_y + cell_height);
			}

			temp_x += less_pixel_x;
			start_x += cell_width;
		}

		start_y += cell_height;
		temp_y += less_pixel_y;
	}

	return image;
}

QImage DrawThread::paintPath()
{
	QImage image = BaseImage;
	QPainter painter(&image);
	QRect r = image.rect().adjusted(1, 1, -1, -1);

	if (showSolution == false) {
		return image;
	}

	int row = maze->getRow();
	int column = maze->getColumn();

	int cell_width = r.width() / column;
	int cell_height = r.height() / row;

	int less_pixel_x = r.width() - cell_width * column;
	int less_pixel_y = r.height() - cell_height * row;

	int start_x = cell_width / 2 + 1;
	int start_y = cell_height / 2 + 1;

	for (auto& current : *path) {
		int rowCnt_first = current.first / column;
		int columnCnt_first = current.first % column;

		int rowCnt_second = current.second / column;
		int columnCnt_second = current.second % column;

		int x_offset = (columnCnt_first * less_pixel_x) / column;
		int y_offset = (rowCnt_first * less_pixel_y) / row;

		painter.setPen(QPen(Qt::green, 2));
		QPoint start(start_x + columnCnt_first * cell_width + x_offset, start_y + rowCnt_first * cell_height + y_offset);
		QPoint end(start_x + columnCnt_second * cell_width + x_offset, start_y + rowCnt_second * cell_height + y_offset);

		painter.drawLine(start, end);
	}

	for (auto& current : *unaccessed) {
		int rowCnt_first = current.first / column;
		int columnCnt_first = current.first % column;

		int rowCnt_second = current.second / column;
		int columnCnt_second = current.second % column;

		int x_offset = (columnCnt_first * less_pixel_x) / column;
		int y_offset = (rowCnt_first * less_pixel_y) / row;

		painter.setPen(QPen(Qt::red, 2));
		
		QPoint start(start_x + columnCnt_first * cell_width + x_offset, start_y + rowCnt_first * cell_height + y_offset);
		QPoint end(start_x + columnCnt_second * cell_width + x_offset, start_y + rowCnt_second * cell_height + y_offset);

		painter.drawLine(start, end);
	}

	return image;
}

void DrawThread::generateMaze(int row, int column)
{
	maze = new AdjacencyList(row, column);


	switch (method)
	{
	case Generate_method::DeepFirstSearch:
		generateMazeByDeepFirstSearch(row, column);
		break;

	case Generate_method::Prim:
		generateMazeByPrim(row, column);
		break;

	case Generate_method::Kruskal:
		generateMazeByKruskal(row, column);
		break;
	}

	isGenerated = true;
	BaseImage = paintMaze();
}

void DrawThread::generateMazeByDeepFirstSearch(int row, int column)
{
	std::vector<int> visited(row * column, 0);
	std::stack<int> stack;
	stack.push(0);
	visited[0] = 1;

	std::random_device rd;
	std::mt19937 g(rd());

	while (!stack.empty())
	{
		int current = stack.top();

		std::vector<int> surround = *maze->getSurround(current);

		std::shuffle(surround.begin(), surround.end(), g);

		//std::random_shuffle(surround.begin(), surround.end());
		//c++17删除了这条特性
		int i = 0;
		for (; i < surround.size(); i++)
		{
			int temp = surround[i];
			if (visited[temp] == 0)
			{
				maze->connect(current, temp);
				visited[temp] = 1;
				stack.push(temp);
				break;
			}
		}

		if (i == surround.size())
		{
			stack.pop();
		}

		QImage image = paintMaze();
		emit imageReady(image);
		//update();
	}
}

void DrawThread::generateMazeByPrim(int row, int column)
{
	std::vector<bool> isConnect(row * column, false);
	std::set<int> connectedDot;

	std::random_device rd;
	std::mt19937 g(rd());

	std::uniform_int_distribution<int> dis(0, row * column - 1);
	connectedDot.insert(0);

	while (!connectedDot.empty())
	{
		int ramdonIndex = dis(g) % connectedDot.size();
		auto it = connectedDot.begin();
		std::advance(it, ramdonIndex);

		int current = *it;

		std::vector<int> surround = *maze->getSurround(current);
		std::shuffle(surround.begin(), surround.end(), g);
		int i = 0;
		for (; i < surround.size(); i++)
		{
			int temp = surround[i];
			if (!isConnect[temp])
			{
				maze->connect(current, temp);
				connectedDot.insert(temp);
				isConnect[temp] = true;

				QImage image = paintMaze();
				emit imageReady(image);
			}
		}

		if (i = surround.size())
		{
			connectedDot.erase(it);
		}

	}
	
	
}

void DrawThread::generateMazeByKruskal(int row, int column)
{
	UnionFind uf(row * column);

	std::random_device rd;
	std::mt19937 g(rd());

	std::vector<std::pair<int, int>> edges;

	for (int i = 0; i < row * column; i++)
	{
		if (i % column != column - 1)
		{
			edges.push_back(std::make_pair(i, i + 1));
		}
		if (i / column != row - 1)
		{
			edges.push_back(std::make_pair(i, i + column));
		}
	}

	std::shuffle(edges.begin(), edges.end(), g);

	for (auto& edge : edges)
	{
		int x = edge.first;
		int y = edge.second;

		if (!uf.isConnected(x, y))
		{
			uf.connect(x, y);
			maze->connect(x, y);
		}

		QImage image = paintMaze();
		emit imageReady(image);
	}
}

void DrawThread::run()
{
	if (isDrawing)
	{
		return;
	}

	isDrawing = true;

	if (isGenerated == false){
		generateMaze(row, column);
	}
	else {
		findPath(0, row * column - 1);
	}
	
	isDrawing = false;

}

DrawThread::DrawThread(MazeWidget* mazeWidget, int row, int column, Generate_method method,FindPath_method findPathMethod)
{
	this->method = method;
	this->findPathMethod = findPathMethod;
	
	this->path = new std::vector<std::pair<int, int>>();
	this->unaccessed = new std::vector<std::pair<int, int>>();
	this->row = row;
	this->column = column;
}

void DrawThread::findPath(int start, int end)
{
	switch (findPathMethod)
	{
	case FindPath_method::DeepFirstSearch:
		findPathByDeepFirstSearch(start, end);
		break;

	case FindPath_method::BreadthFirstSearch:
		findPathByBreadthFirstSearch(start, end);
		break;

	case FindPath_method::Dijkstra:
		findPathByDijkstra(start, end);
		break;

	case FindPath_method::AStar:
		findPathByAStar(start, end);
		break;
	}
}

void DrawThread::findPathByDeepFirstSearch(int start, int end)
{
	path = new std::vector<std::pair<int, int>>();
	unaccessed = new std::vector<std::pair<int, int>>();

	std::vector<int> visited(row * column, 0);
	path->push_back(std::make_pair(0, 0));

	std::random_device rd;
	std::mt19937 g(rd());

	while (true) {
		auto current = path->back();
		if (current.second == end) {
			break;
		}

		visited[current.second] = 1;
		auto neighbor = *maze->getNeighbor(current.second);
		std::shuffle(neighbor.begin(), neighbor.end(), g);
		int i = 0;
		for (; i < neighbor.size(); i++) {
			if (visited[neighbor[i]] == 0) {
				path->push_back(std::make_pair(current.second, neighbor[i]));
				break;
			}
		}

		if (i == neighbor.size()) {
			unaccessed->push_back(path->back());
			path->pop_back();
		}

		QImage image = paintPath();
		emit imageReady(image);
	}

}

void DrawThread::findPathByBreadthFirstSearch(int start, int end)
{
	path = new std::vector<std::pair<int, int>>();
	unaccessed = new std::vector<std::pair<int, int>>();
	int currentVisitPathIndex = 0;

	std::vector<int> visited(row * column, 0);
	std::vector<int> isUnaccessed(row * column, 0);
	std::vector<int> parent(row * column, -1);
	path->push_back(std::make_pair(0, 0));

	std::random_device rd;
	std::mt19937 g(rd());

	while (true) {
		if (currentVisitPathIndex == path->size()) {
			break;
		}

		auto current = path->at(currentVisitPathIndex);

		if (current.second == end) {
			currentVisitPathIndex++;
			continue;
		}

		visited[current.second] = 1;
		auto neighbor = *maze->getNeighbor(current.second);
		std::shuffle(neighbor.begin(), neighbor.end(), g);
		int i = 0;
		bool unaccessedFlag = true;
		for (; i < neighbor.size(); i++) {
			if (visited[neighbor[i]] == 0) {
				if (isUnaccessed[neighbor[i]] == 0)
					unaccessedFlag = false;
				parent[neighbor[i]] = current.second;
				path->push_back(std::make_pair(current.second, neighbor[i]));
			}
		}

		int index = current.second;
		while (unaccessedFlag)
		{
			if (parent[index] == end || parent[index] == 0) {
				break;
			}
			unaccessed->push_back(std::make_pair(parent[index], index));
			isUnaccessed[index] = 1;
			index = parent[index];
			auto neighbors = *maze->getNeighbor(index);
			for (auto& item : neighbors)
			{
				if (item == parent[index])
				{
					continue;
				}

				if (isUnaccessed[item] == 0)
				{
					unaccessedFlag = false;
					break;
				}
			}
		}

		currentVisitPathIndex++;
		QImage image = paintPath();
		emit imageReady(image);
	}
}

void DrawThread::findPathByDijkstra(int start, int end)
{
}

void DrawThread::findPathByAStar(int start, int end)
{
}

void MainWindow::onMethodChanged(Generate_method method)
{
	drawThread->method = method;
	label_generate_method->setText("Generation method: \n" + QString(generate_method_str[(int)method]));
}

void MainWindow::onComboboxFindPathMethodChanged(int index)
{
	switch (combobox_findpath_method->currentIndex())
	{
	case 0:
		drawThread->findPathMethod = FindPath_method::DeepFirstSearch;
		break;
	case 1:
		drawThread->findPathMethod = FindPath_method::BreadthFirstSearch;
		break;
	case 2:
		drawThread->findPathMethod = FindPath_method::Dijkstra;
		break;
	case 3:
		drawThread->findPathMethod = FindPath_method::AStar;
		break;
	default:
		break;
	}
}

void MainWindow::findPathButtonClicked()
{
	if (drawThread->isDrawing)
	{
		return;
	}
	drawThread->isFindingPath = false;
	drawThread->row = row_cnt->value();
	drawThread->column = col_cnt->value();
	drawThread->start();
}
