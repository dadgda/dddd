#include "GR/Node.hxx"
#include "GR/Element.hxx"
#include "GR/Document.hxx"
#include "GR/NotFoundError.hxx"
#include "GR/HierarchyRequestError.hxx"
#include "GR/TypeError.hxx"
#include "GR/util.hxx"
#include <algorithm>
#include <map>
#include <tuple>

GR::Node::Node(Type type, const std::shared_ptr<GR::Document> &owner_document)
    : m_type(type), m_owner_document(owner_document)
{
}

GR::Node::Type GR::Node::nodeType() const
{
  return m_type;
}

bool GR::Node::isConnected() const
{
  return getRootNode()->nodeType() == GR::Node::Type::DOCUMENT_NODE;
}

std::shared_ptr<GR::Document> GR::Node::ownerDocument()
{
  return std::const_pointer_cast<GR::Document>(const_cast<const GR::Node *>(this)->ownerDocument());
}

std::shared_ptr<const GR::Document> GR::Node::ownerDocument() const
{
  if (nodeType() == Type::DOCUMENT_NODE)
    {
      return nullptr;
    }
  return m_owner_document.lock();
}

std::shared_ptr<GR::Document> GR::Node::nodeDocument()
{
  if (m_type == Type::DOCUMENT_NODE)
    {
      return std::dynamic_pointer_cast<GR::Document>(shared_from_this());
    }
  return ownerDocument();
}

std::shared_ptr<const GR::Document> GR::Node::nodeDocument() const
{
  if (m_type == Type::DOCUMENT_NODE)
    {
      return std::dynamic_pointer_cast<const GR::Document>(shared_from_this());
    }
  return ownerDocument();
}

template <typename T> std::shared_ptr<T> getRootNode_impl(std::shared_ptr<T> node)
{
  auto parent_node = node->parentNode();
  if (!parent_node)
    {
      return node;
    }
  return getRootNode_impl(parent_node);
}


std::shared_ptr<GR::Node> GR::Node::getRootNode()
{
  return getRootNode_impl(shared_from_this());
}

std::shared_ptr<const GR::Node> GR::Node::getRootNode() const
{
  return getRootNode_impl(shared_from_this());
}

std::shared_ptr<GR::Node> GR::Node::parentNode()
{
  return m_parent_node.lock();
}

std::shared_ptr<const GR::Node> GR::Node::parentNode() const
{
  return m_parent_node.lock();
}

std::shared_ptr<GR::Element> GR::Node::parentElement()
{
  return std::dynamic_pointer_cast<GR::Element>(parentNode());
}

std::shared_ptr<const GR::Element> GR::Node::parentElement() const
{
  return std::dynamic_pointer_cast<const GR::Element>(parentNode());
}

bool GR::Node::hasChildNodes() const
{
  return !m_child_nodes.empty();
}

std::vector<std::shared_ptr<GR::Node>> GR::Node::childNodes()
{
  return {m_child_nodes.cbegin(), m_child_nodes.cend()};
}

std::vector<std::shared_ptr<const GR::Node>> GR::Node::childNodes() const
{
  return {m_child_nodes.cbegin(), m_child_nodes.cend()};
}

std::shared_ptr<GR::Node> GR::Node::firstChild()
{
  return std::const_pointer_cast<GR::Node>(const_cast<const GR::Node *>(this)->firstChild());
}

std::shared_ptr<const GR::Node> GR::Node::firstChild() const
{
  if (m_child_nodes.empty())
    {
      return nullptr;
    }
  return m_child_nodes.front();
}


std::shared_ptr<GR::Node> GR::Node::lastChild()
{
  return std::const_pointer_cast<GR::Node>(const_cast<const GR::Node *>(this)->lastChild());
}

std::shared_ptr<const GR::Node> GR::Node::lastChild() const
{
  if (m_child_nodes.empty())
    {
      return nullptr;
    }
  return m_child_nodes.back();
}

std::shared_ptr<GR::Node> GR::Node::previousSibling()
{
  return std::const_pointer_cast<GR::Node>(const_cast<const GR::Node *>(this)->previousSibling());
}

std::shared_ptr<const GR::Node> GR::Node::previousSibling() const
{
  auto parent_node = parentNode();
  if (!parent_node)
    {
      return nullptr;
    }
  if (parent_node->m_child_nodes.front().get() == this)
    {
      return nullptr;
    }
  auto it = std::find(parent_node->m_child_nodes.begin(), parent_node->m_child_nodes.end(), shared_from_this());
  --it;
  return *it;
}

std::shared_ptr<GR::Node> GR::Node::nextSibling()
{
  return std::const_pointer_cast<GR::Node>(const_cast<const GR::Node *>(this)->nextSibling());
}

std::shared_ptr<const GR::Node> GR::Node::nextSibling() const
{
  auto parent_node = parentNode();
  if (!parent_node)
    {
      return nullptr;
    }
  if (parent_node->m_child_nodes.back().get() == this)
    {
      return nullptr;
    }
  auto it = std::find(parent_node->m_child_nodes.begin(), parent_node->m_child_nodes.end(), shared_from_this());
  ++it;
  return *it;
}

std::shared_ptr<GR::Node> GR::Node::cloneNode()
{
  // hardcoded in overloaded function as virtual functions may not have default arguments
  return cloneNode(false);
}

std::shared_ptr<GR::Node> GR::Node::cloneNode(bool deep)
{
  auto clone = cloneIndividualNode();
  clone->m_parent_node = {};
  if (deep)
    {
      clone->m_child_nodes.clear();
      for (auto const &child_node : m_child_nodes)
        {
          clone->appendChild(child_node->cloneNode(deep));
        }
    }
  else
    {
      clone->m_child_nodes.clear();
    }
  return clone;
}

bool GR::Node::isEqualNode(const std::shared_ptr<const GR::Node> &otherNode) const
{
  return isSameNode(otherNode);
}

bool GR::Node::isSameNode(const std::shared_ptr<const GR::Node> &otherNode) const
{
  return (otherNode == shared_from_this());
}

bool GR::Node::contains(const std::shared_ptr<const GR::Node> &otherNode) const
{
  if (!otherNode)
    {
      return false;
    }
  if (otherNode->parentNode().get() == this)
    {
      return true;
    }
  return contains(otherNode->parentNode());
}

std::shared_ptr<GR::Node> GR::Node::insertBefore(std::shared_ptr<GR::Node> node, const std::shared_ptr<GR::Node> &child)
{
  if (nodeType() != Type::DOCUMENT_NODE && nodeType() != Type::ELEMENT_NODE)
    {
      throw HierarchyRequestError("parent must be Document or Element node");
    }
  if (!node)
    {
      throw TypeError("node is null");
    }
  if (node->nodeType() != Type::ELEMENT_NODE && node->nodeType() != Type::COMMENT_NODE)
    {
      throw HierarchyRequestError("node must be Element or Comment node");
    }
  if (node.get() == this || node->contains(shared_from_this()))
    {
      throw HierarchyRequestError("node must not be an inclusive ancestor of parent");
    }
  if (child && child->parentNode().get() != this)
    {
      throw NotFoundError("child is not a child of parent");
    }
  if (nodeType() == Type::DOCUMENT_NODE)
    {
      auto this_as_document = dynamic_cast<GR::Document *>(this);
      if (node->nodeType() == Type::ELEMENT_NODE && this_as_document->childElementCount() != 0)
        {
          throw HierarchyRequestError("parent already has an element node");
        }
    }
  nodeDocument()->adoptNode(node);
  if (child)
    {
      auto it = std::find(m_child_nodes.begin(), m_child_nodes.end(), child);
      m_child_nodes.insert(it, node);
    }
  else
    {
      m_child_nodes.push_back(node);
    }
  node->m_parent_node = shared_from_this();
  return node;
}

std::shared_ptr<GR::Node> GR::Node::appendChild(std::shared_ptr<GR::Node> node)
{
  insertBefore(node, nullptr);
  return node;
}

std::shared_ptr<GR::Node> GR::Node::replaceChild(std::shared_ptr<GR::Node> node, const std::shared_ptr<GR::Node> &child)
{
  if (nodeType() != Type::DOCUMENT_NODE && nodeType() != Type::ELEMENT_NODE)
    {
      throw HierarchyRequestError("parent must be Document or Element node");
    }
  if (!node)
    {
      throw TypeError("node is null");
    }
  if (node->nodeType() != Type::ELEMENT_NODE && node->nodeType() != Type::COMMENT_NODE)
    {
      throw HierarchyRequestError("node must be Element or Comment node");
    }
  if (node->contains(shared_from_this()))
    {
      throw HierarchyRequestError("node must not be an inclusive ancestor of parent");
    }
  if (!child)
    {
      throw TypeError("child is null");
    }
  if (child->parentNode().get() != this)
    {
      throw NotFoundError("child is not a child of parent");
    }
  if (nodeType() == Type::DOCUMENT_NODE)
    {
      auto this_as_document = dynamic_cast<GR::Document *>(this);
      if (node->nodeType() == Type::ELEMENT_NODE && this_as_document->firstChildElement() != child)
        {
          throw HierarchyRequestError("parent already has an element node");
        }
    }
  nodeDocument()->adoptNode(node);
  auto it = std::find(m_child_nodes.begin(), m_child_nodes.end(), child);
  *it = node;
  node->m_parent_node = shared_from_this();
  child->m_parent_node = {};
  return node;
}

std::shared_ptr<GR::Node> GR::Node::removeChild(std::shared_ptr<GR::Node> child)
{
  if (!child)
    {
      throw TypeError("child is null");
    }
  if (child->parentNode().get() != this)
    {
      throw NotFoundError("child is not a child of this node");
    }
  m_child_nodes.remove(child);
  child->m_parent_node = {};
  return child;
}

void GR::Node::prepend_impl(const std::vector<std::shared_ptr<GR::Node>> &nodes)
{
  auto previous_first_child = firstChild();
  for (const auto &node : nodes)
    {
      if (previous_first_child)
        {
          insertBefore(node, previous_first_child);
        }
      else
        {
          appendChild(node);
        }
    }
}

void GR::Node::append_impl(const std::vector<std::shared_ptr<GR::Node>> &nodes)
{
  for (const auto &node : nodes)
    {
      appendChild(node);
    }
}

void GR::Node::replaceChildren_impl(const std::vector<std::shared_ptr<GR::Node>> &nodes)
{
  for (const auto &node : m_child_nodes)
    {
      node->m_parent_node = {};
    }
  m_child_nodes.clear();
  append_impl(nodes);
}

void GR::Node::set_owner_document_recursive(const std::shared_ptr<GR::Node> &node,
                                            const std::shared_ptr<GR::Document> &document)
{
  node->m_owner_document = document;
  for (const auto &child_node : node->m_child_nodes)
    {
      set_owner_document_recursive(child_node, document);
    }
}

bool GR::Node::children_are_equal_recursive(const std::shared_ptr<const GR::Node> &left_node,
                                            const std::shared_ptr<const GR::Node> &right_node)
{
  if (left_node == right_node)
    {
      return true;
    }
  if (!left_node || !right_node)
    {
      return false;
    }
  if (left_node->m_child_nodes.size() != right_node->m_child_nodes.size())
    {
      return false;
    }
  for (auto left_it = left_node->m_child_nodes.cbegin(), right_it = right_node->m_child_nodes.cbegin();
       left_it != left_node->m_child_nodes.cend() && right_it != right_node->m_child_nodes.cend();
       ++left_it, ++right_it)
    {
      if (!(*left_it)->isEqualNode(*right_it))
        {
          return false;
        }
    }
  return true;
}


template <typename T, typename U> static std::vector<std::shared_ptr<T>> children_impl(U &child_nodes)
{
  std::vector<std::shared_ptr<T>> child_elements;
  for (const auto &child_node : child_nodes)
    {
      if (child_node->nodeType() == GR::Node::Type::ELEMENT_NODE)
        {
          child_elements.push_back(std::dynamic_pointer_cast<T>(child_node));
        }
    }
  return child_elements;
}

std::vector<std::shared_ptr<GR::Element>> GR::Node::children_impl()
{
  return ::children_impl<GR::Element>(m_child_nodes);
}

std::vector<std::shared_ptr<const GR::Element>> GR::Node::children_impl() const
{
  return ::children_impl<const GR::Element>(m_child_nodes);
}

template <typename T, typename U> static std::shared_ptr<T> firstChildElement_impl(U &child_nodes)
{
  for (const auto &child_node : child_nodes)
    {
      if (child_node->nodeType() == GR::Node::Type::ELEMENT_NODE)
        {
          return std::dynamic_pointer_cast<T>(child_node);
        }
    }
  return nullptr;
}

std::shared_ptr<GR::Element> GR::Node::firstChildElement_impl()
{
  return ::firstChildElement_impl<GR::Element>(m_child_nodes);
}

std::shared_ptr<const GR::Element> GR::Node::firstChildElement_impl() const
{
  return ::firstChildElement_impl<const GR::Element>(m_child_nodes);
}

template <typename T, typename U> static std::shared_ptr<T> lastChildElement_impl(U &child_nodes)
{
  for (auto node_it = child_nodes.rbegin(); node_it != child_nodes.rend(); ++node_it)
    {
      const auto &child_node = *node_it;
      if (child_node->nodeType() == GR::Node::Type::ELEMENT_NODE)
        {
          return std::dynamic_pointer_cast<T>(child_node);
        }
    }
  return nullptr;
}

std::shared_ptr<GR::Element> GR::Node::lastChildElement_impl()
{
  return ::lastChildElement_impl<GR::Element>(m_child_nodes);
}

std::shared_ptr<const GR::Element> GR::Node::lastChildElement_impl() const
{
  return ::lastChildElement_impl<const GR::Element>(m_child_nodes);
}

unsigned long GR::Node::childElementCount_impl() const
{
  unsigned long count = 0;
  for (const auto &node : m_child_nodes)
    {
      if (node->nodeType() == GR::Node::Type::ELEMENT_NODE)
        {
          count++;
        }
    }
  return count;
}

std::shared_ptr<GR::Element> GR::Node::previousElementSibling_impl()
{
  auto sibling = previousSibling();
  while (sibling && sibling->nodeType() != GR::Node::Type::ELEMENT_NODE)
    {
      sibling = sibling->previousSibling();
    }
  return std::dynamic_pointer_cast<Element>(sibling);
}

std::shared_ptr<const GR::Element> GR::Node::previousElementSibling_impl() const
{
  auto sibling = previousSibling();
  while (sibling && sibling->nodeType() != GR::Node::Type::ELEMENT_NODE)
    {
      sibling = sibling->previousSibling();
    }
  return std::dynamic_pointer_cast<const Element>(sibling);
}

std::shared_ptr<GR::Element> GR::Node::nextElementSibling_impl()
{
  auto sibling = nextSibling();
  while (sibling && sibling->nodeType() != GR::Node::Type::ELEMENT_NODE)
    {
      sibling = sibling->nextSibling();
    }
  return std::dynamic_pointer_cast<Element>(sibling);
}

std::shared_ptr<const GR::Element> GR::Node::nextElementSibling_impl() const
{
  auto sibling = nextSibling();
  while (sibling && sibling->nodeType() != GR::Node::Type::ELEMENT_NODE)
    {
      sibling = sibling->nextSibling();
    }
  return std::dynamic_pointer_cast<const Element>(sibling);
}

template <typename T, typename U>
static std::vector<std::shared_ptr<T>> getElementsByClassName_impl(U &node, const std::string &classNames)
{
  if (classNames.empty())
    {
      return {};
    }
  auto class_names = GR::split(GR::tolower(classNames), " ");
  for (auto &class_name : class_names)
    {
      class_name = GR::tolower(GR::strip(class_name));
    }
  for (auto class_name_it = class_names.begin(); class_name_it != class_names.end();)
    {
      if ((*class_name_it).empty())
        {
          class_name_it = class_names.erase(class_name_it);
        }
      else
        {
          ++class_name_it;
        }
    }
  if (class_names.empty())
    {
      return {};
    }
  std::vector<std::shared_ptr<T>> found_elements;
  for (const auto &child_node : node.childNodes())
    {
      if (child_node->nodeType() != GR::Node::Type::ELEMENT_NODE)
        {
          continue;
        }
      const auto &child_element = std::dynamic_pointer_cast<T>(child_node);
      if (child_element)
        {
          auto child_class_names_value = child_element->getAttribute("class");
          if (child_class_names_value.isString())
            {
              auto child_class_names = GR::split((std::string)child_class_names_value, " ");
              if (!child_class_names.empty())
                {
                  for (auto &child_class_name : child_class_names)
                    {
                      child_class_name = GR::tolower(GR::strip(child_class_name));
                    }

                  bool missing_a_class = false;
                  for (const auto &class_name : class_names)
                    {
                      if (std::find(child_class_names.begin(), child_class_names.end(), class_name) ==
                          child_class_names.end())
                        {
                          missing_a_class = true;
                          break;
                        }
                    }
                  if (!missing_a_class)
                    {
                      found_elements.push_back(child_element);
                    }
                }
            }
          auto child_found_elements = child_element->getElementsByClassName(classNames);
          found_elements.insert(found_elements.end(), child_found_elements.begin(), child_found_elements.end());
        }
    }
  return found_elements;
}

std::vector<std::shared_ptr<GR::Element>> GR::Node::getElementsByClassName_impl(const std::string &classNames)
{
  return ::getElementsByClassName_impl<GR::Element>(*this, classNames);
}

std::vector<std::shared_ptr<const GR::Element>>
GR::Node::getElementsByClassName_impl(const std::string &classNames) const
{
  return ::getElementsByClassName_impl<const GR::Element>(*this, classNames);
}

bool GR::Node::matchSelector(const std::shared_ptr<GR::Selector> &selector,
                             std::map<std::tuple<const GR::Element *, const GR::Selector *>, bool> &match_map) const
{
  auto element = dynamic_cast<const GR::Element *>(this);
  if (nodeType() != GR::Node::Type::ELEMENT_NODE || !element)
    {
      return false;
    }
  return selector->matchElement(*element, match_map);
}

void GR::Node::querySelectorsAll_impl(const std::shared_ptr<GR::Selector> &selector,
                                      std::vector<std::shared_ptr<GR::Element>> &found_elements,
                                      std::map<std::tuple<const GR::Element *, const GR::Selector *>, bool> &match_map)
{
  if (matchSelector(selector, match_map))
    {
      found_elements.push_back(std::dynamic_pointer_cast<Element>(shared_from_this()));
    }
  for (auto &child_node : m_child_nodes)
    {
      child_node->querySelectorsAll_impl(selector, found_elements, match_map);
    }
}

void GR::Node::querySelectorsAll_impl(
    const std::shared_ptr<GR::Selector> &selector, std::vector<std::shared_ptr<const GR::Element>> &found_elements,
    std::map<std::tuple<const GR::Element *, const GR::Selector *>, bool> &match_map) const
{
  if (matchSelector(selector, match_map))
    {
      found_elements.push_back(std::dynamic_pointer_cast<const Element>(shared_from_this()));
    }
  for (auto &child_node : m_child_nodes)
    {
      child_node->querySelectorsAll_impl(selector, found_elements, match_map);
    }
}

std::shared_ptr<GR::Element>
GR::Node::querySelectors_impl(const std::shared_ptr<GR::Selector> &selector,
                              std::map<std::tuple<const GR::Element *, const GR::Selector *>, bool> &match_map)
{
  if (matchSelector(selector, match_map))
    {
      return std::dynamic_pointer_cast<Element>(shared_from_this());
    }
  for (auto &child_node : m_child_nodes)
    {
      auto result = child_node->querySelectors_impl(selector, match_map);
      if (result)
        {
          return result;
        }
    }
  return nullptr;
}

std::shared_ptr<const GR::Element>
GR::Node::querySelectors_impl(const std::shared_ptr<GR::Selector> &selector,
                              std::map<std::tuple<const GR::Element *, const GR::Selector *>, bool> &match_map) const
{
  if (matchSelector(selector, match_map))
    {
      return std::dynamic_pointer_cast<const Element>(shared_from_this());
    }
  for (auto &child_node : m_child_nodes)
    {
      auto result = child_node->querySelectors_impl(selector, match_map);
      if (result)
        {
          return result;
        }
    }
  return nullptr;
}
