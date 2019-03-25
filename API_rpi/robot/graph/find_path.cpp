/*
 * Function: FindShortestPath
 * Usage: Vector<arcT *> path = FindShortestPath(start, finish);
 * -------------------------------------------------------------
 * Uses Dijkstra's algorithm to find the shortest path from the
 * start to the finish node. The path is returned as a vector of
 * arc pointers. If no path exists, the result is an empty vector.
 */
Vector<arcT *> FindShortestPath(nodeT *start, nodeT *finish) {
	Vector<arcT *> path;
	Queue< Vector<arcT *> > queue;
	Map<double> fixed;
	while (start != finish) {
		if (!fixed.containsKey(start->name)) {
			fixed.put(start->name, TotalPathDistance(path));
			foreach (arcT *arc in start->arcs) {
				if (!fixed.containsKey(arc->finish->name)) {
					Vector<arcT *> newPath = path;
					newPath.add(arc);
					queue.enqueue(newPath, TotalPathDistance(newPath));
				}
			}
		}
		if (queue.isEmpty()) return Vector<arcT *>();
		path = queue.dequeue();
		start = path[path.size() - 1]->finish;
	}
return path;
}
/*
* Function: TotalPathDistance
* Usage: double distance = TotalPathDistance(path);
* -------------------------------------------------
* Returns the total distance along the path.
*/
double TotalPathDistance(Vector<arcT *> path) {
	double distance = 0;
	foreach (arcT *arc in path) {
		distance += arc->cost;
	}
	return distance;
}