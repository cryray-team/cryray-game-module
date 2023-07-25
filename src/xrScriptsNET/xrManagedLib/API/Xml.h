#pragma once
#include "../../xrXML/xrXMLParser.h"
#include "Filesystem.h"

namespace XRay
{
	/// <summary>X-Ray CXml wrapper</summary>
	public ref class Xml
	{
	internal:
		CXml* pNativeXml;

	public:
		/// <summary>X-Ray XMLNode wrapper</summary>
		ref class Node
		{
		internal:
			XML_NODE* pNativeNode;

			Node(XML_NODE* node);

		public:
			property IntPtr NativeNode
			{
				IntPtr get()
				{
					return (IntPtr)pNativeNode;
				}
			}

			property bool IsHasChild
			{
				bool get()
				{
					return pNativeNode->FirstChild();
				}
			}

			property String^ Value
			{
				virtual String^ get()
				{
					return gcnew String(pNativeNode->Value());
				}

				virtual void set(String^ value)
				{
					string256 Value = {};

					ConvertDotNetStringToAscii(value, Value);

					pNativeNode->SetValue(Value);
				}
			}

			property Node^ Parent
			{
				virtual Node^ get()
				{
					return gcnew Node(pNativeNode->Parent());
				}
			}

			property Node^ First
			{
				virtual Node^ get()
				{
					return gcnew Node(pNativeNode->FirstChild());
				}
			}

			property Node^ Last
			{
				virtual Node^ get()
				{
					return gcnew Node(pNativeNode->LastChild());
				}
			}

			property Node^ Next
			{
				virtual Node^ get()
				{
					return gcnew Node(pNativeNode->NextSibling());
				}
			}

			property Node^ Previous
			{
				virtual Node^ get()
				{
					return gcnew Node(pNativeNode->PreviousSibling());
				}
			}

			virtual ~Node();
		};;

		property IntPtr NativeXml
		{
			IntPtr get()
			{
				return (IntPtr)pNativeXml;
			}
		}

		property Node^ Root
		{
			virtual Node^ get()
			{
				return gcnew Node(pNativeXml->GetRoot());
			}
		}

		property Node^ LocalRoot
		{
			virtual Node^ get()
			{
				return gcnew Node(pNativeXml->GetLocalRoot());
			}

			virtual void set(Node^ node)
			{
				pNativeXml->SetLocalRoot(node->pNativeNode);
			}
		}

		/// <summary>Constructor for Xml</summary>
		/// <param name="path">Path to folder relative to the path to game configs</param>
		/// <param name="fileName">Name of xml file</param>
		Xml(String^ path, String^ fileName);

		/// <summary>Constructor for Xml</summary>
		/// <param name="pathAlias">Path alias to the folder relative to the path to game</param>
		/// <param name="path">Path to folder relative to path alias</param>
		/// <param name="fileName">Name of xml file</param>
		Xml(String^ pathAlias, String^ path, String^ fileName);

		virtual ~Xml();

		/// <summary>
		/// Retrieve the value from the node by name. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Node name which value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		String^ Read(String^ node, int index, String^ defaultValue);

		/// <summary>
		/// Retrieve the value from the child node by name. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node value will be retrieved</param>
		/// <param name="childNode">Child node name which value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		String^ Read(Node^ node, String^ childNode, int index, String^ defaultValue);

		/// <summary>
		/// Retrieve the value from the node. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Node which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		String^ Read(Node^ node, String^ defaultValue);

		/// <summary>
		/// Retrieve the integer value from the node by name. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Node name which value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		int ReadInt(String^ node, int index, int defaultValue);

		/// <summary>
		/// Retrieve the integer value from the child node by name. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node value will be retrieved</param>
		/// <param name="childNode">Child node name which value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		int ReadInt(Node^ node, String^ childNode, int index, int defaultValue);

		/// <summary>
		/// Retrieve the integer value from the node. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Node which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		int ReadInt(Node^ node, int defaultValue);

		/// <summary>
		/// Retrieve the float value from the node by name. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Node name which value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		float ReadFloat(String^ node, int index, float defaultValue);

		/// <summary>
		/// Retrieve the float value from the child node by name. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node value will be retrieved</param>
		/// <param name="childNode">Child node name which value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		float ReadFloat(Node^ node, String^ childNode, int index, float defaultValue);

		/// <summary>
		/// Retrieve the float value from the node. This method will return default value if the value is not defined.
		/// </summary>
		/// <param name="node">Node which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the value is not defined</param>
		float ReadFloat(Node^ node, float defaultValue);

		/// <summary>
		///	Retrieve the value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node name which attribute value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		String^ ReadAttribute(String^ node, int index, String^ attribute, String^ defaultValue);

		/// <summary>
		///	Retrieve the value for the attribute by name from the child node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node attribute value will be retrieved</param>
		/// <param name="childNode">Child node name which attribute will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		String^ ReadAttribute(Node^ node, String^ childNode, int index, String^ attribute, String^ defaultValue);

		/// <summary>
		///	Retrieve the value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node which attribute value will be retrieved</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		String^ ReadAttribute(Node^ node, String^ attribute, String^ defaultValue);

		/// <summary>
		///	Retrieve the bool value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node name which attribute value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		bool ReadAttributeBool(String^ node, int index, String^ attribute, bool defaultValue);

		/// <summary>
		///	Retrieve the bool value for the attribute by name from the child node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node attribute value will be retrieved</param>
		/// <param name="childNode">Child node name which attribute will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		bool ReadAttributeBool(Node^ node, String^ childNode, int index, String^ attribute, bool defaultValue);

		/// <summary>
		///	Retrieve the bool value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node which attribute value will be retrieved</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		bool ReadAttributeBool(Node^ node, String^ attribute, bool defaultValue);

		/// <summary>
		///	Retrieve the integer value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node name which attribute value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		int ReadAttributeInt(String^ node, int index, String^ attribute, int defaultValue);

		/// <summary>
		///	Retrieve the integer value for the attribute by name from the child node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node attribute value will be retrieved</param>
		/// <param name="childNode">Child node name which attribute will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		int ReadAttributeInt(Node^ node, String^ childNode, int index, String^ attribute, int defaultValue);

		/// <summary>
		///	Retrieve the integer value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node which attribute value will be retrieved</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		int ReadAttributeInt(Node^ node, String^ attribute, int defaultValue);

		/// <summary>
		///	Retrieve the float value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node name which attribute value will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		float ReadAttributeFloat(String^ node, int index, String^ attribute, float defaultValue);

		/// <summary>
		///	Retrieve the float value for the attribute by name from the child node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Parent node which child node attribute value will be retrieved</param>
		/// <param name="childNode">Child node name which attribute will be retrieved</param>
		/// <param name="index">TODO</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		float ReadAttributeFloat(Node^ node, String^ childNode, int index, String^ attribute, float defaultValue);

		/// <summary>
		///	Retrieve the float value for the attribute by name from the node. This method will return default value if the attribute is not defined.
		/// </summary>
		/// <param name="node">Node which attribute value will be retrieved</param>
		/// <param name="attribute">Attribute name which value will be retrieved</param>
		/// <param name="defaultValue">Value to return if the attribute is not defined</param>
		float ReadAttributeFloat(Node^ node, String^ attribute, float defaultValue);

		/// <summary>
		///	Returns the count of the child nodes inside the node by names.
		/// </summary>
		/// <param name="node">Parent node name which child node count is returned</param>
		/// <param name="childNode">Child node name the count of which is returned</param>
		/// <param name="index">TODO</param>
		int GetNodesCount(String^ node, String^ childNode, int index);

		/// <summary>
		///	Returns the count of the child nodes by name inside the node.
		/// </summary>
		/// <param name="node">Parent node which child node count is returned</param>
		/// <param name="childNode">Child node name the count of which is returned</param>
		int GetNodesCount(Node^ node, String^ childNode);

		/// <summary>
		///	Navigate to the node by name.
		/// </summary>
		/// <param name="node">Node name to be navigated</param>
		/// <param name="index">TODO</param>
		Node^ NavigateToNode(String^ node, int index);

		/// <summary>
		///	Navigate to the child node by name inside node.
		/// </summary>
		/// <param name="node">Parent node name which inside be navigated</param>
		/// <param name="childNode">Child node name to be navigated</param>
		/// <param name="index">TODO</param>
		Node^ NavigateToNode(Node^ node, String^ childNode, int index);

		/// <summary>
		///	Navigate to the node by name with attribute and attribute value.
		/// </summary>
		/// <param name="node">Node name to be navigated</param>
		/// <param name="attribute">Attribute to navigate</param>
		/// <param name="attributeValue">Attribute value to navigate</param>
		Node^ NavigateToNode(String^ node, String^ attribute, String^ attributeValue);
	};
}